/*
  types.c
  pewter_lang

  Created by Christopher Loonam on 11/19/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>
#include "var.h"
#include "array.h"
#include "dict.h"
#include "types.h"
#include "func.h"

struct var *idx(struct array *args) {
    struct var *idx, *arr;
    
    arr = arrobj(args, 0);
    idx = arrobj(args, 1);
    
    if (arr->type == V_ARR)
        return arrobj(arr->val.aval, (unsigned)idx->val.lval);
    else if (arr->type == V_DIC)
        return dictobj(arr->val.dval, idx->val.sval);
    return NULL;
}

struct var *toint(struct array *args) {
    struct var *a;
    a = arrobj(args, 0);
    if (a->type != V_INT) {
        a->type = V_INT;
        a->val.lval = strtold(a->val.sval, NULL);
    }
    return a;
}

struct var *tostr(struct array *args) {
    struct var *a;
    a = arrobj(args, 0);
    if (a->type == V_INT) {
        a->type = V_STR;
        asprintf(&a->val.sval, "%Lf", a->val.lval);
    }
    return a;
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
}
