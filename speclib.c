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

struct var *divd(struct array *args) {
    struct var *v, *ret;
    unsigned i;
    
    ret = calloc(1, sizeof(struct var));
    ret->name = "__div__";
    ret->type = ((struct var *)arrobj(args, 0))->type;
    
    for (i = 0; i < arrcnt(args); i++) {
        v = arrobj(args, i);
        if (i == 0)
            ret->val.lval = v->val.lval;
        else
            ret->val.lval /= v->val.lval;
    }
    return ret;
}

struct var *mult(struct array *args) {
    struct var *v, *ret;
    unsigned i;
    
    ret = calloc(1, sizeof(struct var));
    ret->name = "__mult__";
    ret->type = ((struct var *)arrobj(args, 0))->type;
    
    for (i = 0; i < arrcnt(args); i++) {
        v = arrobj(args, i);
        if (i == 0)
            ret->val.lval = v->val.lval;
        else
            ret->val.lval *= v->val.lval;
    }
    return ret;
}

struct var *add(struct array *args) {
    struct var *v, *ret;
    unsigned i;
    
    ret = calloc(1, sizeof(struct var));
    ret->name = "__add__";
    ret->type = ((struct var *)arrobj(args, 0))->type;
    for (i = 0; i < arrcnt(args); i++) {
        v = arrobj(args, i);
        
        if (ret->type == V_INT) {
            ret->val.lval += v->val.lval;
        }
        else if (ret->type == V_STR) {
            if (!ret->val.sval)
                ret->val.sval = calloc(1, 1);
            ret->val.sval = realloc(ret->val.sval, strlen(ret->val.sval) + strlen(v->val.sval) + 1);
            strcat(ret->val.sval, v->val.sval);
        }
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
                printf("%Lf", v->val.lval);
                break;
                
            case V_STR:
                printf("%s", v->val.sval);
                
            default:
                break;
        }
    }
    
    return NULL;
}

struct var *idx(struct array *args) {
    struct var *idx, *arr;
    
    arr = arrobj(args, 0);
    idx = arrobj(args, 1);
    
    return arrobj(arr->val.aval, (unsigned)idx->val.lval);
}
