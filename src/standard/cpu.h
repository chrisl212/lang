/*
 cpu.h
 pewter_lang
 
 Created by Christopher Loonam on 11/20/15.
   Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef __pewter_lang__cpu__
#define __pewter_lang__cpu__

#include <stdio.h>

struct dict;
struct array;
struct var;

struct var *slp(struct array *);
void cpu_register(struct dict *);

#endif /* defined(__pewter_lang__cpu__) */
