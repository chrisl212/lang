/*
 func.c
 pewter_lang
 
 Created by Christopher Loonam on 11/22/15.
 Copyright (c) 2015 Christopher Loonam. All rights reserved.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "../foundation/foundation.h"

struct func *strtofunc(const char *s, struct func *f, struct expr *expr, struct dict *funcs) {
    char *argstr, *funcname, *tok, *funcpy;
    struct func *call, *funcarg;
    struct token *atok;
    struct var *arg;
    
    tok = strdup(s);
    
    while (isspace(*tok)) tok++; /* remove leading whitespace */
    if (!strpbrk(tok, " ")) {
        call = dictobj(funcs, tok);
        if (!call) {
            fprintf(stderr, "No such function %s\nLine: %s\n", funcname, expr->expr);
            return NULL;
        }
        call->args = arrnew(NULL);
        return call;
    }
    argstr = strdup(strpbrk(tok, " "));
    funcname = strtok(tok, " ");
    call = dictobj(funcs, funcname);
    if (!call) {
        fprintf(stderr, "No such function %s\nLine: %s\n", funcname, expr->expr);
        return NULL;
    }
    call->args = arrnew(NULL);
    arg = NULL;
    
    atok = gettok(argstr);
    /* funcname becomes the name of the arg here */
    while (atok != NULL) {
        funcname = atok->tok;
        switch (atok->type) {
            case T_LIT:
                arg = strtolit(atok->tok, f, expr);
                arg->name = funcname;
                break;
            case T_VAR:
                arg = dictobj(f->scope, funcname);
                if (!arg) {
                    fprintf(stderr, "No such variable %s\nLine: %s\n", funcname, expr->expr);
                }
                break;
            case T_FUNC:
                funcpy = strdup(atok->tok);
                funcpy++;
                funcpy[strlen(funcpy) - 1] = 0;
                funcarg = strtofunc(funcpy, f, expr, funcs);
                if (funcarg->type == F_NORM)
                    arg = exec(funcarg, funcs);
                else if (funcarg->type == F_SPEC)
                    arg = funcarg->spec(funcarg->args);
                break;
                
            default:
                atok = atok->next;
                continue;
        }
        arradd(call->args, arg);
        atok = atok->next;
    }
    
    return call;
}
