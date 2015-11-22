/*
//  func.h
//  pewter_lang
//
//  Created by Christopher Loonam on 11/16/15.
//  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef pewter_lang_func_h
#define pewter_lang_func_h

struct expr;
struct var;
struct dict;
struct array;

struct func {
    char *name;
    struct array *args;
    struct array *argnms;
    struct dict *scope;
    struct expr *expr;
    struct func *next;
    enum {
        F_NORM,
        F_SPEC,
        F_CTL
    } type;
    struct var *(*spec)(struct array *args);
};

#endif
