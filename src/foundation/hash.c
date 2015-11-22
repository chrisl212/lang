/*
  hash.c
  foundation

  Created by Christopher Loonam on 11/8/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include "hash.h"

uint32_t hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++))
        hash = ((hash << 5) + hash) + (unsigned long)c; /* hash * 33 + c */
    
    return (uint32_t)hash;
}
