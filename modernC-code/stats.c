#include "stats.h"

#define EXTERNAL(NAME) extern typeof(NAME) NAME

EXTERNAL(stats_collect);
EXTERNAL(stats_collect0);
EXTERNAL(stats_collect1);
EXTERNAL(stats_collect2);
EXTERNAL(stats_collect3);
EXTERNAL(stats_samples);
EXTERNAL(stats_mean);
EXTERNAL(stats_sdev);
EXTERNAL(stats_rsdev);
EXTERNAL(stats_skew);
EXTERNAL(stats_var_unbiased);
EXTERNAL(stats_sdev_unbiased);
EXTERNAL(stats_rsdev_unbiased);
