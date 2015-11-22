/*
 types.h
 pewter_lang
 
 Created by Christopher Loonam on 11/19/15.
   Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef __pewter_lang__types__
#define __pewter_lang__types__

#include <stdio.h>

struct dict;
struct var;
struct array;

struct var *idx(struct array *);
struct var *toint(struct array *);
struct var *tostr(struct array *);
struct var *copy(struct array *);
struct var *count(struct array *);
void types_register(struct dict *);

#endif /* defined(__pewter_lang__types__) */
