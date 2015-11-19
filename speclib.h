/*
 speclib.h
 pewter_lang
 
 Created by Christopher Loonam on 11/17/15.
   Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef __pewter_lang__speclib__
#define __pewter_lang__speclib__

#include <stdio.h>

struct var;
struct array;

struct var *minus(struct array *);
struct var *divd(struct array *);
struct var *mult(struct array *);
struct var *add(struct array *);
struct var *specprnt(struct array *);
struct var *idx(struct array *);
struct var *input(struct array *);
struct var *grtr(struct array *);
struct var *less(struct array *);
struct var *eq(struct array *);
struct var *noteq(struct array *);
struct var *toint(struct array *);

#endif /* defined(__pewter_lang__speclib__) */
