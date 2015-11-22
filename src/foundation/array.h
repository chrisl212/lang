/*
 array.h
 foundation
 
 Created by Christopher Loonam on 11/8/15.
   Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef __foundation__array__
#define __foundation__array__

#include <stdio.h>

/**
 @brief @c array_t is a structure that holds an ordered collection of objects of any type.
 */
typedef struct array array_t;

/**
 @brief Creates a new zero-indexed array.
 @param o The objects the array will hold. This list must be terminated with @c NULL; if this is not done, the behavior is undefined.
 @return A newly allocated array containing the objects passed as arguments; if no objects are passed in the function call, an empty array is returned. This array is dynamically allocated and should be freed with a call to @c arrfree()
 */
array_t * arrnew(void *o, ...);

/**
 @brief Returns the object located at index @c idx.
 @param arr The array from which the object will be returned.
 @param idx The index of the object to be returned.
 @return If @c idx is less than the count of the array, the object at that index will be returned. If @c idx is greater than or equal to the count of the array, @c NULL will be returned, as the index will be out of the range of the array.
 */
void * arrobj(array_t *arr, unsigned idx);

/**
 @brief Appends an object to the end of an array.
 @param arr The array to which the object will be added.
 @param o The object to append to the array. If this argument is @c NULL, the array will go unmodified.
 */
void arradd(array_t *arr, void *o);

/**
 @brief Removes the object located at index @c idx from the array.
 @param arr The array from which the object will be removed.
 @param idx The index of the object to be removed. If the index is outside of the bounds of the array, the array will go unmodified.
 */
void arrrm(array_t *arr, unsigned idx);

/**
 @brief Returns the number of objects that are contained in the array @c arr.
 @param arr The array whose count will be returned.
 @return The count of the array @c arr.
 */
unsigned arrcnt(array_t *arr);

/**
 @brief Frees the memory pointed to by the array.
 @param arr The array pointer whose memory will be freed.
 */
void arrfree(array_t *arr);

#endif /* defined(__foundation__array__) */
