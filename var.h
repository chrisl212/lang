/*
//  var.h
//  pewter_lang
//
//  Created by Christopher Loonam on 11/16/15.
//  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef pewter_lang_var_h
#define pewter_lang_var_h

struct array;
struct dict;

struct var {
    char *name;
    enum {
        V_STR,
        V_INT,
        V_ARR,
        V_DIC,
        V_BOOL
    } type;
    union {
        char *sval;
        long double lval;
        struct array *aval;
        struct dict *dval;
        unsigned int bval;
    } val;
};

#endif
