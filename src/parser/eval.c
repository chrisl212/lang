/*
 eval.c
 pewter_lang
 
 Created by Christopher Loonam on 11/16/15.
 Copyright (c) 2015 Christopher Loonam. All rights reserved.
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "standard.h"
#include "eval.h"
#include "token.h"
#include "expr.h"
#include "func.h"
#include "var.h"
#include "array.h"
#include "dict.h"
#include "hash.h"

void standard(struct dict *);

struct var *exec(struct func *f, struct dict *funcs) {
    struct expr *expr;
    struct token *tok;
    struct var *v, *callres, *arg;
    struct func *call, *ctl;
    char *aname, *tokcp;
    unsigned i;
    size_t len;
    
    i = 0;
    if (!f->scope)
        f->scope = dictnew(NULL, NULL);
    while (i < arrcnt(f->args)) {
        aname = arrobj(f->argnms, i);
        v = arrobj(f->args, i++);
        if (aname)
            dictadd(f->scope, v, aname);
    }
    if (f->type == F_NORM) {
        v = calloc(1, sizeof(struct var));
        v->type = V_ARR;
        v->val.aval = f->args;
        v->name = "args";
        
        dictadd(f->scope, v, "args");
    }
    
    arg = NULL;
    for (expr = f->expr; expr != NULL; expr = expr->next) {
        
        for (tok = expr->tok; tok != NULL; tok = tok->next) {
            if (!tok->tok)
                break;
            if (strcmp(tok->tok, "func") == 0 && tok->next->type == T_FUNC) {
                expr->type = E_DECL;
                continue;
            }
            else if (f->type == F_CTL && strcmp(tok->tok, "stop") == 0)
                return NULL;
            else if (tok->type == T_FUNC && expr->type != E_DECL && *(tok->tok) == '(') {
                tokcp = strdup(tok->tok);
                tokcp++;
                len = strlen(tokcp);
                tokcp[len-1] = 0;
                
                call = strtofunc(tokcp, f, expr, funcs);
                
                if (call->type == F_NORM || call->type == F_CTL)
                    callres = exec(call, funcs);
                else
                    callres = call->spec(call->args);
                if (expr->next && strcmp(expr->next->tok->tok, "end") == 0)
                    return callres;
            }
            else if (tok->type == T_CTL) {
                if (strcmp(tok->tok, "while") == 0) {
                    tok = tok->next;
                    tokcp = strdup(tok->tok);
                    tokcp++;
                    tokcp[strlen(tokcp) - 1] = 0;
                    call = strtofunc(tokcp, f, expr, funcs);
                    while ((callres = (call->type == F_SPEC)?call->spec(call->args):exec(call, funcs))->val.bval) {
                        ctl = calloc(1, sizeof(struct func));
                        ctl->expr = expr->next;
                        ctl->type = F_CTL;
                        ctl->args = arrnew(NULL);
                        ctl->scope = f->scope;
                        exec(ctl, funcs);
                        call = strtofunc(tokcp, f, expr, funcs);
                    }
                    while (!strstr(expr->expr, "stop"))
                        expr = expr->next;
                }
                else if (strcmp(tok->tok, "if") == 0) {
                    tok = tok->next;
                    tokcp = strdup(tok->tok);
                    tokcp++;
                    tokcp[strlen(tokcp) - 1] = 0;
                    call = strtofunc(tokcp, f, expr, funcs);
                    if ((callres = (call->type == F_SPEC)?call->spec(call->args):exec(call, funcs))->val.bval) {
                        ctl = calloc(1, sizeof(struct func));
                        ctl->expr = expr->next;
                        ctl->type = F_CTL;
                        ctl->args = arrnew(NULL);
                        ctl->scope = f->scope;
                        exec(ctl, funcs);
                        call = strtofunc(tokcp, f, expr, funcs);
                    }
                    while (!strstr(expr->expr, "stop"))
                        expr = expr->next;
                }
            }
            else if (expr->next && strcmp(expr->next->tok->tok, "end") == 0 && tok->type == T_VAR) {
                return dictobj(f->scope, tok->tok);
            }
            else if (tok->type == T_VAR) {
                if (!(v = dictobj(f->scope, tok->tok))) {
                    v = calloc(1, sizeof(struct var));
                    v->name = tok->tok;
                    dictadd(f->scope, v, v->name);
                }
                if (tok->next && tok->next->type == T_ASS) {
                    tok = tok->next->next;
                    if (tok->type == T_LIT) {
                        callres = strtolit(tok->tok, f, expr);
                        v->type = callres->type;
                        v->val = callres->val;
                    }
                    else if (tok->type == T_FUNC) {
                        tokcp = strdup(tok->tok);
                        tokcp++;
                        len = strlen(tokcp);
                        tokcp[len-1] = 0;
                        
                        call = strtofunc(tokcp, f, expr, funcs);
                        
                        if (call->type == F_NORM)
                            callres = exec(call, funcs);
                        else
                            callres = call->spec(call->args);
                        v->type = callres->type;
                        v->val = callres->val;
                        continue;
                    }
                    else if (tok->type == T_VAR) {
                        callres = dictobj(f->scope, tok->tok);
                        v->type = callres->type;
                        v->val = callres->val;
                    }
                }
            }
        }
    }
    
    return NULL;
}

int eval(const char *s, int argc, char **argv) {
    struct expr *ex;
    struct func *entry;
    struct var *v;
    int i;
    struct array *strngs;
    struct dict *funcs;
    
    strngs = arrnew(NULL);
    
    funcs = dictnew(NULL, NULL);
    standard(funcs);
    
    ex = getexpr(s);
    parse(ex, funcs);
    
    entry = dictobj(funcs, "main");
    
    for (i = 0; i < argc; i++) {
        v = calloc(1, sizeof(struct var));
        v->name = "__arg__";
        v->type = V_STR;
        v->val.sval = argv[i];
        arradd(strngs, v);
    }
    
    v = arrobj(entry->args, 0);
    if (v) {
        v->type = V_INT;
        v->val.ival = (long)argc;
    }
    
    v = arrobj(entry->args, 1);
    if (v) {
        v->type = V_ARR;
        v->val.aval = strngs;
    }
    
    exec(entry, funcs);
    return 1;
}

void standard(struct dict *funcs) {
    arith_register(funcs);
    io_register(funcs);
    types_register(funcs);
    condit_register(funcs);
    cpu_register(funcs);
}
