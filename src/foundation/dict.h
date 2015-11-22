/*
 dict.h
 foundation
 
 Created by Christopher Loonam on 11/8/15.
   Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef __foundation__dict__
#define __foundation__dict__

#include <stdio.h>

/**
 @brief @c dict_t is a structure that holds a set of keys, each with an associated object. The implementation utilizes a hash table to improve the speed.
 */
typedef struct dict dict_t;

/**
 @brief Creates and returns a dictionary object, containing the keys and their associated objects that are passed as arguments. The arguments
 @param obj The objects to be stored in the dictionary.
 @param key The key to associate with @c obj, which is the argument that immediately precedes it.
 @return A new dictionary object whose memory is dynamically allocated using @c calloc(). This should be freed after usage.
 */
dict_t * dictnew(void *obj, const char *key, ...); /* obj, key, terminate with NULL */

/**
 @brief Returns the object associated with the key @c key that is located in the dictionary @c dict. The search is performed using a hash table, thereby increasing the speed of the search.
 @param dict The dictionary which will be searched for the key.
 @param key The key to match in the dictionary.
 @return The object associated with @c key, if one exists. If one does not exist, @c NULL is returned.
 */
void * dictobj(dict_t *dict, const char *key);

/**
 @brief Adds the object @c obj to the dictionary @c dict and associates it with the key @c key. If that key is already associated with a different object in the dictionary, it is changed to be associated with @c obj.
 @param dict The dictionary to which the object will be added. 
 @param obj The object which will be added to the dictionary.
 @param key The key to associate with @c obj.
 */
void dictadd(dict_t *dict, void *obj, const char *key); /* obj, key */

/**
 @brief Returns the number of unique key/value pairs contained in the dictionary.
 @param dict The dictionary whose count will be returned.
 @return The number of unique key/value pairs located in @c dict.
 */
unsigned dictcnt(dict_t *dict);

/**
 @brief Frees the memory occupied by the dictionary @c dict and the memory occupied by the hash table it uses to store its values.
 @param dict The dictionary whose memory will be freed.
 */
void dictfree(dict_t *dict);

#endif /* defined(__foundation__dict__) */
