#ifndef CIRCULAR_H
# define CIRCULAR_H 1

# include "c23-fallback.h"
# include <stdio.h>

typedef struct circular circular;

/** @brief a type for a circular buffer for @c double values
 **
 ** This data structure allows to add @c double values in rear and to
 ** take them out in front. Each such structure has a maximal amount
 ** of elements that can be stored in it.
 **
 **/
/*
 *
 * Direct access to all the elements is deprecated.
 */
struct circular {
  size_t  start [[deprecated("privat")]]; /* First element     */
  size_t  len   [[deprecated("privat")]]; /* Number of elements*/
  size_t  cap   [[deprecated("privat")]]; /* Maximum capacity  */
  double* tab   [[deprecated("privat")]]; /* Data array        */
};

/**
 ** @name Initialization and destruction
 ** @{
 **/

/** @brief Initialize a circular buffer @a c with maximally @a
 ** cap elements.
 **
 ** Only use this function on an uninitialized buffer.
 **
 ** Each buffer that is initialized with this function must be
 ** destroyed with a call to ::circular_destroy.
 **/
circular* circular_init(circular* c, size_t cap);

/** @brief Destroy circular buffer @a c
 **
 ** @a c must have been initialized with a call to ::circular_init
 **/
void circular_destroy(circular* c);

/** @brief Delete circular buffer @a c
 **
 ** @a c must have been allocated with a call to ::circular_new
 **/
inline
void circular_delete(circular* c) {
  circular_destroy(c);
  free(c);
}
/**
 ** @}
 **/

/** @brief Allocate and initialize a circular buffer with maximally
 ** @a len elements.
 **
 ** Each buffer that is allocated with this function must be deleted
 ** with a call to ::circular_delete.
 **/
[[nodiscard("pointer to allocated data dropped")]]
[[__gnu__::__malloc__, __gnu_free__(circular_delete)]]
inline
circular* circular_new(size_t len) {
  return circular_init(malloc(sizeof(circular)), len);
}

/**
 ** @name Using elements of a circular
 ** @{
 **/

/** @brief Append a new element with value @a value to the buffer @a c
 **
 ** @return c if the new element could be appended, null otherwise.
 **/
circular* circular_append(circular* c, double value);

/** @brief Remove the oldest element from @a c and return its value
 **
 ** @return the removed element if it exists, @c 0.0 otherwise.
 **/
double circular_pop(circular* c);

/** @brief Return a pointer to position @a pos in buffer @a c
 **
 ** @return a pointer to element @a pos of the buffer, null otherwise.
 **/
double* circular_element(circular const* c, size_t pos);


/**
 ** @}
 **/

/**
 ** @name Maintain a circular
 ** @{
 **/

/** @brief Return the number of elements stored. */
size_t circular_getlength(circular const* c);

/** @brief Resize to capacity @a cap. */
[[nodiscard("returned pointer replaces function argument")]]
circular* circular_resize(circular* c, size_t cap);

/** @brief Print the buffer values to stream @a s. */
void circular_fput(circular* c, FILE* s);

/**
 ** @}
 **/



#endif
