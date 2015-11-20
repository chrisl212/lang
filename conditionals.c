/*
  conditionals.c
  pewter_lang

  Created by Christopher Loonam on 11/19/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include "conditionals.h"
#include <string.h>
#include <stdlib.h>
#include "var.h"
#include "dict.h"
#include "array.h"
#include "func.h"

typedef enum {
    LESS,
    GREATER,
    LESSEQ,
    GREATEREQ,
    EQ,
    NOTEQ
} comparison;

struct var *comp(struct array *, comparison);

struct var *comp(struct array *args, comparison t) {
    struct var *ret, *a1, *a2;
    a1 = arrobj(args, 0);
    a2 = arrobj(args, 1);
    
    ret = calloc(1, sizeof(struct var));
    ret->type = V_BOOL;
    
    switch (t) {
        case LESS:
            ret->val.bval = (a1->val.lval < a2->val.lval);
            break;
            
        case LESSEQ:
            ret->val.bval = (a1->val.lval <= a2->val.lval);
            break;
            
        case GREATER:
            ret->val.bval = (a1->val.lval > a2->val.lval);
            break;
            
        case GREATEREQ:
            ret->val.bval = (a1->val.lval >= a2->val.lval);
            break;
            
        case EQ:
            ret->val.bval = (a1->val.lval == a2->val.lval);
            break;
            
        case NOTEQ:
            ret->val.bval = (a1->val.lval != a2->val.lval);
            break;
            
        default:
            break;
    }
    return ret;
}

struct var *grtr(struct array *args) {
    return comp(args, GREATER);
}

struct var *less(struct array *args) {
    return comp(args, LESS);
}

struct var *grteq(struct array *args) {
    return comp(args, GREATEREQ);
}

struct var *lesseq(struct array *args) {
    return comp(args, LESSEQ);
}

struct var *eq(struct array *args) {
    return comp(args, EQ);
}

struct var *noteq(struct array *args) {
    return comp(args, NOTEQ);
}

void condit_register(struct dict *funcs) {
    struct func *f;
    
    f = calloc(1, sizeof(struct func));
    f->name = ">";
    f->type = F_SPEC;
    f->spec = grtr;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = ">=";
    f->type = F_SPEC;
    f->spec = grteq;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "<";
    f->type = F_SPEC;
    f->spec = grtr;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "<=";
    f->type = F_SPEC;
    f->spec = lesseq;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "=";
    f->type = F_SPEC;
    f->spec = eq;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "!=";
    f->type = F_SPEC;
    f->spec = noteq;
    dictadd(funcs, f, f->name);
}
