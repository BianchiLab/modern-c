#include "c23-fallback.h"
#include <tgmath.h>
#include <stdio.h>

/**
 ** @file
 ** @brief Collect some simple statistics online as we go.
 **
 ** This uses a generalization of Welford's trick to compute running
 ** mean and variance. See
 **
 ** Philippe Pébay. Formulas for robust, one-pass parallel computation
 ** of covariances and arbitrary-order statistical moments. Technical
 ** Report SAND2008-6212, SANDIA, 2008. URL
 ** http://prod.sandia.gov/techlib/access-control.cgi/2008/086212.pdf.
 **/

/**
 ** @brief A simple data structure to collect the 0th to 3rd moment of
 ** a statistic.
 **
 ** @warning Since this also uses a @c double for the number of
 ** samples, the validity of all this is restricted to about
 ** @f$2^{50} \approx 10^{15}@f$ samples.
 **/
struct stats {
  double moment[4];
};
typedef struct stats stats;


/**
 ** @brief Return the number of samples that had been entered into the
 ** statistic @a c.
 **/
inline
double stats_samples(stats c[static 1]) [[__unsequenced__]] {
  return c->moment[0];
}

/**
 ** @brief Return the mean value of the samples that had been entered
 ** into the statistic @a c.
 **/
inline
double stats_mean(stats c[static 1]) [[__unsequenced__]] {
  return c->moment[1];
}

/**
 ** @brief Return the variance of the samples that had been entered
 ** into the statistic @a c.
 **/
inline
double stats_var(stats c[static 1]) [[__reproducible__]] {
  return c->moment[2]/stats_samples(c);
}

// Clang has this notorious design flaw in <tgmath.h> where they
// declare interfaces as being static. In some cases this results in
// an avalanche of confusing messages of static functions that would
// be used in an inline function.
#if __clang_major__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wstatic-in-inline"
#endif

/**
 ** @brief Return the standard deviation of the samples that had been
 ** entered into the statistic @a c.
 **/
inline
double stats_sdev(stats c[static 1]) [[__reproducible__]] {
  return sqrt(stats_var(c));
}

/**
 ** @brief Return the relative standard deviation of the samples that
 ** had been entered into the statistic @a c.
 **/
inline
double stats_rsdev(stats c[static 1]) [[__reproducible__]] {
  return sqrt(stats_var(c))/stats_mean(c);
}

/**
 ** @brief Return the normalized skew of the samples that had been
 ** entered into the statistic @a c.
 **/
inline
double stats_skew(stats c[static 1]) [[__reproducible__]] {
  double var = stats_var(c);
  return (c->moment[3]/pow(var, 1.5))/stats_samples(c);
}

/**
 ** @brief Return the unbiased variance of the samples that had been
 ** entered into the statistic @a c.
 **
 ** Use Bessel's correction to have an estimation of the unbiased
 ** variance of the overall population.
 **/
inline
double stats_var_unbiased(stats c[static 1]) [[__reproducible__]] {
  return c->moment[2]/(stats_samples(c)-1);
}

/**
 ** @brief Return the unbiased standard deviation of the samples that
 ** had been entered into the statistic @a c.
 **
 ** Use Bessel's correction to have an less biased estimation of the
 ** variance of the overall population.
 **/
inline
double stats_sdev_unbiased(stats c[static 1]) [[__reproducible__]] {
  return sqrt(stats_var_unbiased(c));
}

#if __clang_major__
#pragma clang diagnostic pop
#endif



/**
 ** @brief Return the unbiased relative standard deviation of the
 ** samples that had been entered into the statistic @a c.
 **/
inline
double stats_rsdev_unbiased(stats c[static 1]) [[__reproducible__]] {
  return stats_rsdev(c)*(1+1/(4*stats_samples(c)));
}

/**
 ** @brief Add value @a val to the statistic @a c.
 **
 ** @c moments is the number of statistic moments that is collected,
 ** it has to be between `0` and `3`, including.
 **/
inline
void stats_collect(stats c[static 1],
                   double val, unsigned moments)
  [[__reproducible__]] {
  double n  = stats_samples(c);
  double n0 = n-1;
  double n1 = n+1;
  double delta0 = 1;
  double delta  = val - stats_mean(c);
  double delta1 = delta/n1;
  double delta2 = delta1*delta*n;
  switch (moments) {
  default:
    c->moment[3] += (delta2*n0 - 3*c->moment[2])*delta1;
    [[__fallthrough__]];
  case 2:
    c->moment[2] += delta2;
    [[__fallthrough__]];
  case 1:
    c->moment[1] += delta1;
    [[__fallthrough__]];
  case 0:
    c->moment[0] += delta0;
  }
}

/**
 ** @brief Add value @a val to the statistic @a c.
 **
 ** Only the number of samples is collected.
 **/
inline
void stats_collect0(stats c[static 1],
                    double val)
  [[__reproducible__]] {
  stats_collect(c, val, 0);
}

/**
 ** @brief Add value @a val to the statistic @a c.
 **
 ** Only the number of samples and mean is collected.
 **/
inline
void stats_collect1(stats c[static 1],
                    double val)
  [[__reproducible__]] {
  stats_collect(c, val, 1);
}

/**
 ** @brief Add value @a val to the statistic @a c.
 **
 ** The number of samples, mean and standard deviation is collected.
 **/
inline
void stats_collect2(stats c[static 1],
                    double val)
  [[__reproducible__]] {
  stats_collect(c, val, 2);
}

/**
 ** @brief Add value @a val to the statistic @a c.
 **
 ** All statistics up to the skew are collected.
 **/
inline
void stats_collect3(stats c[static 1],
                    double val)
  [[__reproducible__]] {
  stats_collect(c, val, 3);
}
