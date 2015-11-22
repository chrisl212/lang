/*
 arithmetic.h
 pewter_lang
 
 Created by Christopher Loonam on 11/19/15.
   Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef __pewter_lang__arithmetic__
#define __pewter_lang__arithmetic__

#include <stdio.h>

struct dict;
struct array;
struct var;

struct var *minus(struct array *);
struct var *divd(struct array *);
struct var *mult(struct array *);
struct var *add(struct array *);
struct var *mod(struct array *);
struct var *ran(struct array *);
struct var *power(struct array *);
void arith_register(struct dict *);

#endif /* defined(__pewter_lang__arithmetic__) */
