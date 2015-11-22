/*
 conditionals.h
 pewter_lang
 
 Created by Christopher Loonam on 11/19/15.
   Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef __pewter_lang__conditionals__
#define __pewter_lang__conditionals__

#include <stdio.h>

struct dict;
struct var;
struct array;

struct var *grtr(struct array *);
struct var *less(struct array *);
struct var *grteq(struct array *);
struct var *lesseq(struct array *);
struct var *eq(struct array *);
struct var *noteq(struct array *);
void condit_register(struct dict *);

#endif /* defined(__pewter_lang__conditionals__) */
