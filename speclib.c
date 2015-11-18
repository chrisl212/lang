/*
  speclib.c
  pewter_lang

  Created by Christopher Loonam on 11/17/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <string.h>
#include <stdlib.h>
#include "var.h"
#include "array.h"
#include "speclib.h"

typedef enum {
    ADD,
    SUB,
    MULT,
    DIVID
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
                    break;
                case MULT:
                    ret->val.lval *= v->val.lval;
                    break;
                case DIVID:
                    ret->val.lval /= v->val.lval;
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

struct var *specprnt(struct array *args) {
    struct var *v;
    unsigned i;
    
    for (i = 0; i < arrcnt(args); i++) {
        v = arrobj(args, i);
        switch (v->type) {
            case V_INT:
                printf("%.0Lf", v->val.lval);
                break;
                
            case V_STR:
                printf("%s", v->val.sval);
                
            default:
                break;
        }
    }
    putchar('\n');
    return NULL;
}

struct var *idx(struct array *args) {
    struct var *idx, *arr;
    
    arr = arrobj(args, 0);
    idx = arrobj(args, 1);
    
    return arrobj(arr->val.aval, (unsigned)idx->val.lval);
}

struct var *input(struct array *args) {
    char *buf, c;
    size_t i;
    struct var *v;
    
    i = 0;
    buf = malloc(1);
    args = NULL;
    while ((c = (char)getchar()) != '\n') {
        buf = realloc(buf, ++i);
        buf[i-1] = c;
    }
    buf = realloc(buf, ++i);
    buf[i-1] = 0;
    
    v = calloc(1, sizeof(struct var));
    v->type = V_STR;
    v->val.sval = buf;
    v->name = "__input__";
    return v;
}
