/*
//  expr.h
//  pewter_lang
//
//  Created by Christopher Loonam on 11/16/15.
//  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef pewter_lang_expr_h
#define pewter_lang_expr_h

struct token;

struct expr {
    char *expr;
    struct token *tok;
    struct expr *next;
    enum {
        E_NORM,
        E_DECL
    } type;
};

#endif
