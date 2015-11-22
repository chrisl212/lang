/*
  types.c
  pewter_lang

  Created by Christopher Loonam on 11/19/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "../foundation/foundation.h"
#include "../parser/parser.h"

struct var *idx(struct array *args) {
    struct var *idx, *arr, *v;
    
    arr = arrobj(args, 0);
    idx = arrobj(args, 1);
    
    if (arr->type == V_ARR)
        return arrobj(arr->val.aval, (idx->type == V_INT) ? (unsigned)idx->val.ival : (unsigned)idx->val.fval);
    else if (arr->type == V_DIC)
        return dictobj(arr->val.dval, idx->val.sval);
    else if (arr->type == V_STR) {
        v = calloc(1, sizeof(struct var));
        v->type = V_STR;
        v->name = "__idx__";
        v->val.sval = malloc(2);
        strncpy(v->val.sval, &arr->val.sval[(idx->type == V_INT) ? (unsigned)idx->val.ival : (unsigned)idx->val.fval], 1);
        v->val.sval[1] = 0;
        return v;
    }
    return NULL;
}

struct var *toint(struct array *args) {
    struct var *a;
    a = arrobj(args, 0);
    if (a->type != V_INT) {
        a->type = V_INT;
        a->val.ival = strtol(a->val.sval, NULL, 10);
    }
    return a;
}

struct var *tostr(struct array *args) {
    struct var *a;
    a = arrobj(args, 0);
    if (a->type == V_DOUB) {
        a->type = V_STR;
        asprintf(&a->val.sval, "%Lf", a->val.fval);
    }
    else if (a->type == V_DOUB) {
        a->type = V_STR;
        asprintf(&a->val.sval, "%ld", a->val.ival);
    }
    return a;
}

struct var *count(struct array *args) {
    struct var *a, *ret;
    
    ret = calloc(1, sizeof(struct var));
    ret->type = V_INT;
    a = arrobj(args, 0);
    switch (a->type) {
        case V_STR:
            ret->val.ival = (long)strlen(a->val.sval);
            break;
            
        case V_ARR:
            ret->val.ival = (long)arrcnt(a->val.aval);
            break;
            
        case V_DIC:
            ret->val.ival = (long)dictcnt(a->val.dval);
            break;
            
        default:
            break;
    }
    
    return ret;
}

struct var *copy(struct array *args) {
    struct var *a, *ret;

    
    ret = calloc(1, sizeof(struct var));
    a = arrobj(args, 0);
    ret->type = a->type;
    switch (a->type) {
        case V_STR:
            ret->val.sval = strdup(a->val.sval);
            break;
            
        case V_ARR:
            /* ret->val.aval = (long)arrcnt(a->val.aval); TODO: implement copy for these */
            break;
            
        case V_DIC:
            ret->val.ival = (long)dictcnt(a->val.dval);
            break;
            
        default:
            break;
    }
    return ret;
}

void types_register(struct dict *funcs) {
    struct func *f;
    
    f = calloc(1, sizeof(struct func));
    f->name = "obj";
    f->type = F_SPEC;
    f->spec = idx;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "int";
    f->type = F_SPEC;
    f->spec = toint;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "str";
    f->type = F_SPEC;
    f->spec = tostr;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "count";
    f->type = F_SPEC;
    f->spec = count;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "copy";
    f->type = F_SPEC;
    f->spec = copy;
    dictadd(funcs, f, f->name);
}
