/*
 eval.h
 pewter_lang
 
 Created by Christopher Loonam on 11/16/15.
   Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef __pewter_lang__eval__
#define __pewter_lang__eval__

#include <stdio.h>

struct func;
struct dict;

int eval(const char *, int, char **);
struct var *exec(struct func *, struct dict *);

#endif /* defined(__pewter_lang__eval__) */
