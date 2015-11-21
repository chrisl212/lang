/*
  arithmetic.c
  pewter_lang

  Created by Christopher Loonam on 11/19/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
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
            ret->val = v->val;
        else {
            switch (op) {
                case ADD:
                    if (ret->type == V_INT) {
                        ret->val.ival += (v->type == V_INT) ? v->val.ival : v->val.fval;
                    }
                    else {
                        ret->val.fval += (v->type == V_INT) ? v->val.ival : v->val.fval;
                    }
                    break;
                case SUB:
                    if (ret->type == V_INT) {
                        ret->val.ival -= (v->type == V_INT) ? v->val.ival : v->val.fval;
                    }
                    else {
                        ret->val.fval -= (v->type == V_INT) ? v->val.ival : v->val.fval;
                    }
                    break;
                case MULT:
                    if (ret->type == V_INT) {
                        ret->val.ival *= (v->type == V_INT) ? v->val.ival : v->val.fval;
                    }
                    else {
                        ret->val.fval *= (v->type == V_INT) ? v->val.ival : v->val.fval;
                    }
                    break;
                case DIVID:
                    if (ret->type == V_INT) {
                        ret->val.ival /= (v->type == V_INT) ? v->val.ival : v->val.fval;
                    }
                    else {
                        ret->val.fval /= (v->type == V_INT) ? v->val.ival : v->val.fval;
                    }
                    break;
                    
                case MOD:
                    if (ret->type == V_INT) {
                        ret->val.ival = ret->val.ival % (long)((v->type == V_INT) ? v->val.ival : v->val.fval);
                    }
                    else {
                        ret->val.fval = (long)ret->val.fval % (long)((v->type == V_INT) ? v->val.ival : v->val.fval);
                    }
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
    if (ret->type == V_INT || ret->type == V_DOUB)
        return arith(args, ADD);
    else if (ret->type == V_STR) {
        for (i = 0; i < arrcnt(args); i++) {
            v = arrobj(args, i);
            
            if (!ret->val.sval)
                ret->val.sval = calloc(1, 1);
            ret->val.sval = realloc(ret->val.sval, strlen(ret->val.sval) + strlen(v->val.sval) + 1);
            strcat(ret->val.sval, v->val.sval);
        }
    }
    else if (ret->type == V_ARR) {
        ret->val.aval = ((struct var *)arrobj(args, 0))->val.aval;
        for (i = 0; i < arrcnt(args); i++) {
            v = arrobj(args, i);
            
            arradd(ret->val.aval, v);
        }
    }

    return ret;
}

struct var *mod(struct array *args) {
    return arith(args, MOD);
}

struct var *ran(struct array *args) {
    struct var *ret, *a;
    static long seed;
    
    if (!seed) {
        seed = (long)time(NULL);
        srand((unsigned int)seed);
    }
    if (arrcnt(args) > 0) {
        a = arrobj(args, 0);
        if (a->type == V_INT || a->type == V_DOUB)
            seed = (long)((a->type == V_INT) ? a->val.ival : a->val.fval);
    }
    
    ret->type = V_INT;
    ret->val.ival = (long)rand();
    
    return ret;
}

struct var *power(struct array *args) {
    long double num, pow;
    struct var *v;
    
    if (arrcnt(args) < 2)
        return NULL;
    v = arrobj(args, 0);
    num = (long)((v->type == V_INT) ? v->val.ival : v->val.fval);
    v = arrobj(args, 1);
    pow = (long)((v->type == V_INT) ? v->val.ival : v->val.fval);

    v = calloc(1, sizeof(struct var));
    v->type = V_DOUB;
    v->val.fval = powl(num, pow);
    v->name = "__pow__";
    
    return v;
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
    
    f = calloc(1, sizeof(struct func));
    f->name = "random";
    f->type = F_SPEC;
    f->spec = ran;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "^";
    f->type = F_SPEC;
    f->spec = power;
    dictadd(funcs, f, f->name);
}
