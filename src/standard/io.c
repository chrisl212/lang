/*
  io.c
  pewter_lang

  Created by Christopher Loonam on 11/19/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "io.h"
#include "../foundation/foundation.h"
#include "../parser/parser.h"

struct var *fiopen(struct array *args) {
    struct var *ret;
    
    if (arrcnt(args) < 1)
        return NULL;
    ret = calloc(1, sizeof(struct var));
    
    ret->type = V_FILE;
    ret->val.sval = ((struct var *)arrobj(args, 0))->val.sval;
    
    return ret;
}

struct var *output(struct array *args) {
    struct var *v;
    unsigned i, j;
    FILE *f;
    
    f = stdout;
    for (j = i = 0; i < arrcnt(args); i++) {
        v = arrobj(args, i);
        if (v->type == V_FILE)
            f = fopen(v->val.sval, "a+"), j = 1;
    }
    
    for (i = 0; i < arrcnt(args); i++) {
        v = arrobj(args, i);
        switch (v->type) {
            case V_DOUB:
                fprintf(f, "%.6Lf", v->val.fval);
                break;
                
            case V_INT:
                fprintf(f, "%ld", v->val.ival);
                break;
                
            case V_STR:
                if (strcmp(v->val.sval, "\\n") == 0) {
                    putc('\n', f);
                    break;
                }
                fprintf(f, "%s", v->val.sval);
                break;
                
            default:
                break;
        }
    }
    fflush(f);
    if (j)
        fclose(f);
    return NULL;
}

struct var *input(struct array *args) {
    char *buf, c;
    size_t i, j;
    struct var *v;
    FILE *f;
    
    f = stdin;
    for (j = i = 0; i < arrcnt(args); i++) {
        v = arrobj(args, (unsigned)i);
        if (v->type == V_FILE)
            f = fopen(v->val.sval, "r"), j = 1;
    }
    
    i = 0;
    buf = malloc(1);
    args = NULL;
    while ((c = (char)getc(f)) != '\n') {
        buf = realloc(buf, ++i);
        buf[i-1] = c;
    }
    buf = realloc(buf, ++i);
    buf[i-1] = 0;
    
    v = calloc(1, sizeof(struct var));
    v->type = V_STR;
    v->val.sval = buf;
    v->name = "__input__";
    
    if (j)
        fclose(f);
    return v;
}

struct var *inread(struct array *args) {
    struct var *ret, *fvar, *num, *v;
    unsigned i;
    FILE *f;
    size_t numbytes;

    ret = calloc(1, sizeof(struct var));
    for (i = 0; i < arrcnt(args); i++) {
        v = arrobj(args, i);
        if (v->type == V_FILE)
            fvar = v;
        else if (v->type == V_INT || v->type == V_DOUB)
            num = v;
    }
    if (!fvar)
        f = stdin;
    else {
        f = fopen(fvar->val.sval, "r");
        if (!f) return NULL;
        if (!num) {
            fseek(f, 0, SEEK_END);
            num = calloc(1, sizeof(struct var));
            num->type = V_INT;
            num->val.ival = (long)ftell(f);
            fseek(f, 0, SEEK_SET);
        }
    }
    if (!num)
        numbytes = 1024;
    else
        numbytes = (num->type == V_INT) ? (size_t)num->val.ival : (size_t)num->val.fval;

    ret->val.sval = malloc(numbytes);
    numbytes = fread(ret->val.sval, 1, numbytes, f);
    ret->val.sval[numbytes] = 0;

    if (fvar)
        fclose(f);
    return ret;
}

void io_register(struct dict *funcs) {
    struct func *f;

    f = calloc(1, sizeof(struct func));
    f->name = "open";
    f->type = F_SPEC;
    f->spec = fiopen;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "write";
    f->type = F_SPEC;
    f->spec = output;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "input";
    f->type = F_SPEC;
    f->spec = input;
    dictadd(funcs, f, f->name);

    f = calloc(1, sizeof(struct func));
    f->name = "read";
    f->type = F_SPEC;
    f->spec = inread;
    dictadd(funcs, f, f->name);
}
