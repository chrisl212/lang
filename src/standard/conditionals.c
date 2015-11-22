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

struct var *compd(struct array *, comparison);
struct var *compf(struct array *, comparison);

struct var *compd(struct array *args, comparison t) {
    struct var *ret, *a1, *a2;
    a1 = arrobj(args, 0);
    a2 = arrobj(args, 1);
    
    ret = calloc(1, sizeof(struct var));
    ret->type = V_BOOL;
    
    switch (t) {
        case LESS:
            ret->val.bval = (a1->val.ival < a2->val.ival);
            break;
            
        case LESSEQ:
            ret->val.bval = (a1->val.ival <= a2->val.ival);
            break;
            
        case GREATER:
            ret->val.bval = (a1->val.ival > a2->val.ival);
            break;
            
        case GREATEREQ:
            ret->val.bval = (a1->val.ival >= a2->val.ival);
            break;
            
        case EQ:
            ret->val.bval = (a1->val.ival == a2->val.ival);
            break;
            
        case NOTEQ:
            ret->val.bval = (a1->val.ival != a2->val.ival);
            break;
            
        default:
            break;
    }
    return ret;
}

struct var *compf(struct array *args, comparison t) {
    struct var *ret, *a1, *a2;
    a1 = arrobj(args, 0);
    a2 = arrobj(args, 1);
    
    ret = calloc(1, sizeof(struct var));
    ret->type = V_BOOL;
    
    switch (t) {
        case LESS:
            ret->val.bval = (a1->val.fval < a2->val.fval);
            break;
            
        case LESSEQ:
            ret->val.bval = (a1->val.fval <= a2->val.fval);
            break;
            
        case GREATER:
            ret->val.bval = (a1->val.fval > a2->val.fval);
            break;
            
        case GREATEREQ:
            ret->val.bval = (a1->val.fval >= a2->val.fval);
            break;
            
        case EQ:
            ret->val.bval = (a1->val.fval == a2->val.fval);
            break;
            
        case NOTEQ:
            ret->val.bval = (a1->val.fval != a2->val.fval);
            break;
            
        default:
            break;
    }
    return ret;
}

struct var *grtr(struct array *args) {
    if (((struct var *)arrobj(args, 0))->type == V_INT)
        return compd(args, GREATER);
    else
        return compf(args, GREATER);
}

struct var *less(struct array *args) {
    if (((struct var *)arrobj(args, 0))->type == V_INT)
        return compd(args, LESS);
    else
        return compf(args, LESS);
}

struct var *grteq(struct array *args) {
    if (((struct var *)arrobj(args, 0))->type == V_INT)
        return compd(args, GREATEREQ);
    else
        return compf(args, GREATEREQ);
}

struct var *lesseq(struct array *args) {
    if (((struct var *)arrobj(args, 0))->type == V_INT)
        return compd(args, LESSEQ);
    else
        return compf(args, LESSEQ);
}

struct var *eq(struct array *args) {
    if (((struct var *)arrobj(args, 0))->type == V_INT)
        return compd(args, EQ);
    else
        return compf(args, EQ);
}

struct var *noteq(struct array *args) {
    if (((struct var *)arrobj(args, 0))->type == V_INT)
        return compd(args, NOTEQ);
    else
        return compf(args, NOTEQ);
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
    f->spec = less;
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
