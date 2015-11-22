/*
  array.c
  foundation

  Created by Christopher Loonam on 11/8/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdlib.h>
#include <stdarg.h>
#include "array.h"

struct array {
    void **objs;
    unsigned cnt;
    unsigned long sz;
};

array_t * arrnew(void *o, ...) {
    array_t *ret;
    va_list lst;
    
    ret = calloc(1, sizeof(struct array));
    ret->objs = malloc(1);
    ret->cnt = 0;
    
    if (!o)
        return ret;
    
    va_start(lst, o);
    
    do arradd(ret, o);
    while ((o = va_arg(lst, void *)));
    
    va_end(lst);
    
    return ret;
}

void * arrobj(array_t *arr, unsigned idx) {
    if (idx >= arr->cnt)
        return NULL;
    return arr->objs[idx];
}

void arradd(array_t *arr, void *o) {
    if (!o)
        return;
    arr->cnt++;
    arr->sz += sizeof(o);
    arr->objs = realloc(arr->objs, arr->sz);
    arr->objs[arr->cnt - 1] = o;
}

void arrrm(array_t *arr, unsigned idx) {
    array_t *new;
    unsigned i;
    
    if (idx >= arr->cnt)
        return;
    
    new = arrnew(NULL);
    
    i = 0;
    while (i < arr->cnt) {
        if (i != idx)
            arradd(new, arrobj(arr, i));
        i++;
    }
    free(arr->objs);
    arr->objs = new->objs;
    arr->cnt = new->cnt;
    arr->sz = new->sz;
    free(new);
}

unsigned arrcnt(array_t *arr) {
    return arr->cnt;
}

void arrfree(array_t *arr) {
    free(arr->objs);
    free(arr);
}
