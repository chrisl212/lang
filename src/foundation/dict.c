/*
  dict.c
  foundation

  Created by Christopher Loonam on 11/8/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "hash.h"
#include "dict.h"

struct item {
    struct item *next;
    unsigned hash;
    char *key;
    void *o;
};

struct dict {
    struct item *head;
    unsigned cnt;
};

struct item * dictitem(dict_t *d, unsigned h);
void itemfree(struct item *);

dict_t * dictnew(void *o, const char *k, ...) {
    dict_t *dic;
    va_list lst;
    
    dic = calloc(1, sizeof(struct dict));
    dic->head = NULL;
    if (!o || !k)
        return dic;
    
    va_start(lst, k); 
    
    do dictadd(dic, o, k);
    while ((o = va_arg(lst, void *)) && (k = va_arg(lst, char *)));
    
    va_end(lst);
    
    return dic;
}

void * dictobj(dict_t *d, const char *k) {
    struct item *i;
    
    i = dictitem(d, (unsigned)hash(k));
    
    return (i)?i->o:NULL;
}

struct item * dictitem(dict_t *d, unsigned h) {
    struct item *i;
    
    i = d->head;
    if (!i)
        return NULL;
    
    do {
        if (i->hash == h)
            return i;
    } while ((i = i->next));
    
    return NULL;
}

void dictadd(dict_t *d, void *o, const char *k) {
    struct item *i;
    
    if ((i = dictitem(d, (unsigned)hash(k)))) {
        i->o = o;
        return;
    }
    
    d->cnt++;
    i = d->head;
    if (i) {
        while (i->next) i = i->next;
        i->next = calloc(1, sizeof(struct item));
        i->next->hash = (unsigned)hash(k);
        i->next->o = o;
        i->next->key = strdup(k);
        return;
    }
    d->head = calloc(1, sizeof(struct item));
    d->head->hash = (unsigned)hash(k);
    d->head->o = o;
    d->head->key = strdup(k);
}

unsigned dictcnt(dict_t *d) {
    return d->cnt;
}

void itemfree(struct item *i) {
    struct item *tmp;
    
    while (i) {
        tmp = i;
        i = i->next;
        
        free(tmp->key);
        free(tmp);
    }
}

void dictfree(dict_t *d) {
    itemfree(d->head);
    free(d);
}
