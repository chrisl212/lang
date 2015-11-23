/*
 io.h
 pewter_lang
 
 Created by Christopher Loonam on 11/19/15.
   Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef __pewter_lang__io__
#define __pewter_lang__io__

#include <stdio.h>

struct dict;
struct var;
struct array;

struct var *fiopen(struct array *);
struct var *input(struct array *);
struct var *output(struct array *);
struct var *inread(struct array *);
void io_register(struct dict *);

#endif /* defined(__pewter_lang__io__) */
