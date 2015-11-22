/*
  cpu.c
  pewter_lang

  Created by Christopher Loonam on 11/20/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdlib.h>
#include <unistd.h>
#include "dict.h"
#include "array.h"
#include "var.h"
#include "func.h"
#include "cpu.h"

struct var *slp(struct array *args) {
    unsigned t;
    struct var *v;
    
    t = 0;
    if (arrcnt(args) > 0) {
        v = arrobj(args, 0);
        if (v->type == V_INT) {
            t = (unsigned)v->val.ival;
        }
        else if (v->type == V_DOUB) {
            t = (unsigned)v->val.fval;
        }
    }
    sleep(t);
    
    return NULL;
}

void cpu_register(struct dict *funcs) {
    struct func *f;
    
    f = calloc(1, sizeof(struct func));
    f->name = "wait";
    f->type = F_SPEC;
    f->spec = slp;
    dictadd(funcs, f, f->name);
}
