/*
 * arraylist.h
 *
 * Dynamic array interface.
 *
 * This implementation makes use of the `hidden header` method by
 * storing metadata in the memory block right before the the first item.
 *
 * Hence, is strongly discouraged to use functions from this module for anything
 * but arraylists allocated with the provided constructors, or else `undefined
 * behaviour` will occur.
 *
 * The API Achieves generics like behaviour through the use of macros,
 * exchanging code length for simplicity and compile time type checking.
 *
 */

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AL_MAXBUF 256
#define AL_INIT_CAPACITY 32
#define AL_MAXSIZE ((~(size_t)0) >> 1)
#define AL_MAXCAP(T) ((AL_MAXSIZE - sizeof(alH_head)) / sizeof(T))
#define AL_GROWTH_FACTOR 2

enum alH_errcode { AL_FAIL, AL_SUCCESS };
#define AL_ERRSIZE (~(size_t)0)
#define AL_ERRTYPE(T) ((T){0})

#define alH_log(message)                                                       \
  (fprintf(stderr, "line %d %s: %s\n", __LINE__, __FUNCTION__, message))

#define alH_nullguard(pointer, message, errcode)                               \
  do {                                                                         \
    if (pointer == NULL) {                                                     \
      alH_log(message);                                                        \
      return errcode;                                                          \
    }                                                                          \
  } while (0)

#define __alH_skiphead(head) ((void *)((alH_head *)(head) + 1))
#define __alH_head(array) ((alH_head *)(array) - 1)

typedef struct alH_head_t {
  size_t length, capacity;
} alH_head;

/*
 * Simply returns error `code` if `AL_ENABLE_TESTS` is defined, else aborts
 * the program.
 */
static inline int alH_fail(int code) {
#ifndef AL_ENABLE_TESTS
  exit(EXIT_FAILURE);
#endif
  return code;
}

// TODO: unwrap as many functions as possible from the macros

/*
 * Returns the current length of an arraylist, `AL_ERRSIZE` in case of failure.
 */
static inline size_t al_length(const void *array) {
  alH_nullguard(array, "NULL pointer", AL_ERRSIZE);
  return __alH_head(array)->length;
}

/*
 * Returns the capacity of an arraylist, `AL_ERRSIZE` in case of failure.
 */
static inline size_t al_capacity(const void *array) {
  alH_nullguard(array, "NULL pointer", AL_ERRSIZE);
  return __alH_head(array)->capacity;
}

/*
 * Frees an arraylist allocated with either `al_create()` or `al_init()`.
 *
 * It is strongly discouraged to use this function for anything but
 * "arraylists", as doing so will cause undefined behaviour.
 *
 * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.
 */
static inline int al_free(void *array) {
  alH_nullguard(array, "NULL pointer", AL_FAIL);
  free(__alH_head(array));
  return AL_SUCCESS;
}

/*
 * Sets items from `start` to `end` to `c`.
 */
static inline void alH_set(void *array, int c, size_t start, size_t end,
                           size_t size) {
  memset((char *)array + (start * size), c, (end - start) * size);
}

#define al_define(name, T)                                                     \
                                                                               \
  /*                                                                           \
   * Allocates an empty arraylist of a specified `capacity`.                   \
   *                                                                           \
   * Returns `NULL` in case of failure.                                        \
   */                                                                          \
  static T *al_create_##name(size_t capacity) {                                \
    if (capacity >= AL_MAXCAP(T)) {                                            \
      alH_log("`capacity` exceeds maximum capacity");                          \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    capacity = capacity == 0 ? AL_INIT_CAPACITY : capacity;                    \
    alH_head *head =                                                           \
        (alH_head *)malloc(sizeof(alH_head) + (capacity * sizeof(T)));         \
    if (!head) {                                                               \
      alH_log("Cannot allocate memory");                                       \
      return NULL;                                                             \
    }                                                                          \
    head->capacity = capacity;                                                 \
    head->length = 0;                                                          \
                                                                               \
    return (T *)__alH_skiphead(head);                                          \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Allocates an empty arraylist of `AL_INIT_CAPACITY`.                       \
   *                                                                           \
   * Returns `NULL` in case of failure.                                        \
   */                                                                          \
  static inline T *al_init_##name() { return al_create_##name(0); }            \
                                                                               \
  /*                                                                           \
   * Resizes the capacity of an arraylist by the specified amount.             \
   *                                                                           \
   * Shrinking the arraylist will permanently delete (if present) all items    \
   * past the new decrased capacity.                                           \
   *                                                                           \
   * Passing in 0 as `capacity` will set the new capacity to                   \
   * `AL_INIT_CAPACITY`.                                                       \
   *                                                                           \
   * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.                     \
   */                                                                          \
  static int al_resize_##name(T *arrptr[], size_t capacity) {                  \
    alH_nullguard(arrptr, "NULL pointer", AL_FAIL);                            \
    alH_nullguard(*arrptr, "pointer to array points to NULL", AL_FAIL);        \
                                                                               \
    if (capacity >= AL_MAXCAP(T)) {                                            \
      alH_log("`capacity` exceeds maximum capacity");                          \
      return AL_FAIL;                                                          \
    }                                                                          \
    alH_head *head = __alH_head(*arrptr), *new_head;                           \
    size_t oldlen = head->length;                                              \
                                                                               \
    capacity = (capacity == 0) ? AL_INIT_CAPACITY : capacity;                  \
    new_head =                                                                 \
        (alH_head *)realloc(head, sizeof(alH_head) + sizeof(T) * capacity);    \
    if (!new_head) {                                                           \
      alH_log("Reallocation failed");                                          \
      return alH_fail(AL_FAIL);                                                \
    }                                                                          \
                                                                               \
    new_head->capacity = capacity;                                             \
    new_head->length = (capacity < oldlen) ? capacity : oldlen;                \
                                                                               \
    *arrptr = (T *)__alH_skiphead(new_head);                                   \
    alH_set(*arrptr, 0, al_length(*arrptr), al_capacity(*arrptr), sizeof(T));  \
    return AL_SUCCESS;                                                         \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Increases the capacity of an arraylist by `AL_GROWTH_FACTOR`.             \
   *                                                                           \
   * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.                     \
   */                                                                          \
  static inline int al_grow_##name(T *arrptr[]) {                              \
    alH_nullguard(arrptr, "NULL pointer", AL_FAIL);                            \
    alH_nullguard(*arrptr, "pointer to array points to NULL", AL_FAIL);        \
    return al_resize_##name(arrptr,                                            \
                            __alH_head(*arrptr)->capacity * AL_GROWTH_FACTOR); \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Appends the specified `item` to the tail of the arraylist.                \
   *                                                                           \
   * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.                     \
   */                                                                          \
  static int al_push_##name(T *arrptr[], const T item) {                       \
    alH_nullguard(arrptr, "NULL pointer", AL_FAIL);                            \
    alH_nullguard(*arrptr, "pointer to array points to NULL", AL_FAIL);        \
                                                                               \
    T *array = *arrptr;                                                        \
                                                                               \
    alH_head *head = __alH_head(array);                                        \
    if (head->length + 1 >= AL_MAXCAP(T)) {                                    \
      alH_log("array reached maximum capacity");                               \
      return AL_FAIL;                                                          \
    }                                                                          \
                                                                               \
    if (head->length >= head->capacity) {                                      \
      if (al_grow_##name(arrptr) != AL_SUCCESS)                                \
        return AL_FAIL;                                                        \
      array = *arrptr;                                                         \
    }                                                                          \
                                                                               \
    array[__alH_head(array)->length++] = (T)item;                              \
                                                                               \
    return AL_SUCCESS;                                                         \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Inserts the specified `item` at `index`, automatically shifting           \
   * all items.                                                                \
   *                                                                           \
   * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.                     \
   */                                                                          \
  static int al_insert_##name(T *arrptr[], size_t index, const T item) {       \
    alH_nullguard(arrptr, "NULL pointer", AL_FAIL);                            \
    alH_nullguard(*arrptr, "pointer to array points to NULL", AL_FAIL);        \
                                                                               \
    T *arr = *arrptr;                                                          \
    if (index > al_length(arr)) {                                              \
      alH_log("`index` exceeds arraylist length");                             \
      return AL_FAIL;                                                          \
    }                                                                          \
    if (al_push_##name(arrptr, item) != AL_SUCCESS)                            \
      return AL_FAIL;                                                          \
                                                                               \
    memmove(&arr[index + 1], &arr[index],                                      \
            sizeof(T) * (al_length(arr) - index));                             \
    arr[index] = (T)item;                                                      \
    return AL_SUCCESS;                                                         \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Returns the index of the `item` if found.                                 \
   *                                                                           \
   * This function accepts a comparator callback, it can be set to `NULL` for  \
   * plain value comparison.                                                   \
   *                                                                           \
   * Returns `AL_ERRSIZE` in case of failure or item not found.                \
   */                                                                          \
  static size_t al_contains_##name(T array[], const T item,                    \
                                   int (*cmp)(const T a, const T b)) {         \
    alH_nullguard(array, "NULL pointer", AL_ERRSIZE);                          \
                                                                               \
    size_t length = al_length(array);                                          \
    for (size_t i = 0; i < length; i++) {                                      \
      int found = (cmp == NULL) ? memcmp(&array[i], &item, sizeof(T)) == 0     \
                                : cmp(array[i], item) == 0;                    \
      if (found) {                                                             \
        return i;                                                              \
      }                                                                        \
    }                                                                          \
    return AL_ERRSIZE;                                                         \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Removes and returns the item at the specified `index`.                    \
   *                                                                           \
   * Returns `AL_ERRTYPE` if no item was found or in case an error was         \
   * encountered.                                                              \
   */                                                                          \
  static inline T al_rmat_##name(T array[], size_t index) {                    \
    alH_nullguard(array, "NULL pointer", AL_ERRTYPE(T));                       \
                                                                               \
    if (index >= al_length(array)) {                                           \
      alH_log("`index` exceeds array length");                                 \
      return AL_ERRTYPE(T);                                                    \
    }                                                                          \
                                                                               \
    T removed = array[index];                                                  \
    memmove(&array[index], &array[index + 1],                                  \
            sizeof(T) * (al_length(array) - index));                           \
    memset(array + al_length(array) - 1, 0, sizeof(T));                        \
    __alH_head(array)->length--;                                               \
    return removed;                                                            \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Removes and returns the first instance of the specified `item` in the     \
   * arraylist.                                                                \
   *                                                                           \
   * This function accepts a comparator callback, it can be set to `NULL` for  \
   * plain value comparison.                                                   \
   *                                                                           \
   * Returns `AL_ERRTYPE` if no item was found or in case an error was         \
   * encountered.                                                              \
   */                                                                          \
  static T al_rm_##name(T array[], const T item,                               \
                        int (*cmp)(const T a, const T b)) {                    \
    alH_nullguard(array, "NULL pointer", AL_ERRTYPE(T));                       \
                                                                               \
    size_t index = al_contains_##name(array, item, cmp);                       \
    if (index == AL_ERRSIZE) {                                                 \
      return AL_ERRTYPE(T);                                                    \
    }                                                                          \
    return al_rmat_##name(array, index);                                       \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Removes all instances of the specified `item` in the arraylist.           \
   *                                                                           \
   * This function accepts a comparator callback, it can be set to `NULL` for  \
   * plain value comparison.                                                   \
   *                                                                           \
   * Returns the number of removed items.                                      \
   */                                                                          \
  static int al_rmall_##name(T array[], const T item,                          \
                             int (*cmp)(const T a, const T b)) {               \
    alH_nullguard(array, "NULL pointer", AL_FAIL);                             \
                                                                               \
    size_t removed = 0, i = 0;                                                 \
    while (i < al_length(array)) {                                             \
      int found = (cmp == NULL) ? memcmp(&array[i], &item, sizeof(T)) == 0     \
                                : cmp(array[i], item) == 0;                    \
      if (found) {                                                             \
        size_t n_found = 1;                                                    \
        for (size_t j = i; j + 1 < al_length(array); j++) {                    \
          int found_adjacent = (cmp == NULL) ? memcmp(&array[i + n_found],     \
                                                      &item, sizeof(T)) == 0   \
                                             : cmp(array[i + 1], item) == 0;   \
          if (found_adjacent)                                                  \
            n_found++;                                                         \
          else                                                                 \
            break;                                                             \
        }                                                                      \
                                                                               \
        memmove(&array[i], &array[i + n_found],                                \
                sizeof(T) * (al_length(array) - i - n_found));                 \
        __alH_head(array)->length -= n_found;                                  \
        removed += n_found;                                                    \
      } else                                                                   \
        i++;                                                                   \
    }                                                                          \
    alH_set(array, 0, al_length(array), al_length(array) + removed,            \
            sizeof(T));                                                        \
    return removed;                                                            \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Removes and returns last item in the arraylist.                           \
   *                                                                           \
   * Returns `AL_ERRTYPE` in case of error.                                    \
   */                                                                          \
  static inline T al_pop_##name(T array[]) {                                   \
    alH_nullguard(array, "NULL pointer", AL_ERRTYPE(T));                       \
    if (al_length(array) == 0) {                                               \
      alH_log("array is empty");                                               \
      return AL_ERRTYPE(T);                                                    \
    }                                                                          \
    return al_rmat_##name(array, al_length(array) - 1);                        \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Prints the contents of an arraylist, followed by a newline separator.     \
   *                                                                           \
   * Callback `int to_string(const T item, char* buf)` should write the        \
   * string representation of an item (max size `AL_MAXBUF`) to the buffer     \
   * `buf`.                                                                    \
   *                                                                           \
   * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.                     \
   */                                                                          \
  static int al_print_##name(const T array[],                                  \
                             int (*to_string)(const T, char *)) {              \
    alH_nullguard(array, "NULL pointer", AL_FAIL);                             \
                                                                               \
    alH_head *head = __alH_head(array);                                        \
    char buffer[AL_MAXBUF] = "\0";                                             \
    printf("{ ");                                                              \
    for (size_t i = 0; i < head->length; i++) {                                \
      if (!to_string || !to_string(array[i], buffer)) {                        \
        alH_log("string conversion failed");                                   \
        return AL_FAIL;                                                        \
      }                                                                        \
      if (i == head->length - 1) {                                             \
        printf("%s ", buffer);                                                 \
      } else {                                                                 \
        printf("%s, ", buffer);                                                \
      }                                                                        \
    }                                                                          \
    puts("}");                                                                 \
                                                                               \
    return AL_SUCCESS;                                                         \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Clears every item in the arraylist.                                       \
   *                                                                           \
   * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.                     \
   */                                                                          \
  static inline int al_clear_##name(T array[]) {                               \
    alH_nullguard(array, "NULL pointer", AL_FAIL);                             \
    alH_set(array, 0, 0, al_length(array), sizeof(T));                         \
    __alH_head(array)->length = 0;                                             \
    return AL_SUCCESS;                                                         \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Returns a shallow copy of `array` or `NULL` in case of failure.           \
   */                                                                          \
  static T *al_copy_##name(T array[]) {                                        \
    alH_nullguard(array, "NULL pointer", NULL);                                \
                                                                               \
    T *copy = al_create_##name(al_capacity(array));                            \
    if (!copy) {                                                               \
      return NULL;                                                             \
    }                                                                          \
    __alH_head(copy)->length = __alH_head(array)->length;                      \
    for (size_t i = 0; i < al_length(array); i++)                              \
      copy[i] = array[i];                                                      \
                                                                               \
    return copy;                                                               \
  }                                                                            \
                                                                               \
  /*                                                                           \
   * Swaps two items in the `array`.                                           \
   *                                                                           \
   * Returns generic error codes `AL_SUCCESS` / `AL_FAIL`.                     \
   */                                                                          \
  static int al_swap_##name(T array[], size_t a, size_t b) {                   \
    alH_nullguard(array, "NULL pointer", AL_FAIL);                             \
                                                                               \
    if (a >= al_length(array)) {                                               \
      alH_log("index `a` out of bounds");                                      \
      return AL_FAIL;                                                          \
    }                                                                          \
    if (b >= al_length(array)) {                                               \
      alH_log("index `b` out of bounds");                                      \
      return AL_FAIL;                                                          \
    }                                                                          \
                                                                               \
    T tmp = array[a];                                                          \
    array[a] = array[b];                                                       \
    array[b] = tmp;                                                            \
                                                                               \
    return AL_SUCCESS;                                                         \
  }

#endif // ARRAYLIST_H
