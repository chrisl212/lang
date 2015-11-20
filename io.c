/*
  io.c
  pewter_lang

  Created by Christopher Loonam on 11/19/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>
#include "var.h"
#include "array.h"
#include "io.h"
#include "dict.h"
#include "func.h"

struct var *output(struct array *args) {
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

void io_register(struct dict *funcs) {
    struct func *f;
    
    f = calloc(1, sizeof(struct func));
    f->name = "output";
    f->type = F_SPEC;
    f->spec = output;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "input";
    f->type = F_SPEC;
    f->spec = input;
    dictadd(funcs, f, f->name);
}
