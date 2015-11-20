/*
  arithmetic.c
  pewter_lang

  Created by Christopher Loonam on 11/19/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>
#include "var.h"
#include "dict.h"
#include "array.h"
#include "arithmetic.h"
#include "func.h"

typedef enum {
    ADD,
    SUB,
    MULT,
    DIVID,
    MOD
} ops;

struct var *arith(struct array *args, ops op);

struct var *arith(struct array *args, ops op) {
    struct var *v, *ret;
    unsigned i;
    
    ret = calloc(1, sizeof(struct var));
    ret->name = "__arith__";
    ret->type = ((struct var *)arrobj(args, 0))->type;
    
    for (i = 0; i < arrcnt(args); i++) {
        v = arrobj(args, i);
        if (i == 0)
            ret->val.lval = v->val.lval;
        else {
            switch (op) {
                case ADD:
                    ret->val.lval += v->val.lval;
                    break;
                case SUB:
                    ret->val.lval -= v->val.lval;
                    ret->type = V_INT;
                    break;
                case MULT:
                    ret->val.lval *= v->val.lval;
                    ret->type = V_INT;
                    break;
                case DIVID:
                    ret->val.lval /= v->val.lval;
                    ret->type = V_INT;
                    break;
                    
                case MOD:
                    ret->val.lval = (unsigned long)ret->val.lval % (unsigned long)v->val.lval;
                    ret->type = V_INT;
                    break;
                    
                default:
                    break;
            }
        }
    }
    return ret;
}

struct var *minus(struct array *args) {
    return arith(args, SUB);
}

struct var *divd(struct array *args) {
    return arith(args, DIVID);
}

struct var *mult(struct array *args) {
    return arith(args, MULT);
}

struct var *add(struct array *args) {
    struct var *v, *ret;
    unsigned i;
    
    ret = calloc(1, sizeof(struct var));
    ret->name = "__add__";
    ret->type = ((struct var *)arrobj(args, 0))->type;
    if (ret->type == V_INT)
        return arith(args, ADD);
    for (i = 0; i < arrcnt(args); i++) {
        v = arrobj(args, i);
        
        if (!ret->val.sval)
            ret->val.sval = calloc(1, 1);
        ret->val.sval = realloc(ret->val.sval, strlen(ret->val.sval) + strlen(v->val.sval) + 1);
        strcat(ret->val.sval, v->val.sval);
    }
    return ret;
}

struct var *mod(struct array *args) {
    return arith(args, MOD);
}

void arith_register(struct dict *funcs) {
    struct func *f;
    
    f = calloc(1, sizeof(struct func));
    f->name = "+";
    f->type = F_SPEC;
    f->spec = add;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "/";
    f->type = F_SPEC;
    f->spec = divd;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "*";
    f->type = F_SPEC;
    f->spec = mult;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "-";
    f->type = F_SPEC;
    f->spec = minus;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "%";
    f->type = F_SPEC;
    f->spec = mod;
    dictadd(funcs, f, f->name);
}
