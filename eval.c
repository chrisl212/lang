/*
  eval.c
  pewter_lang

  Created by Christopher Loonam on 11/16/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "speclib.h"
#include "eval.h"
#include "token.h"
#include "expr.h"
#include "func.h"
#include "var.h"
#include "array.h"
#include "dict.h"
#include "hash.h"

#undef bool
#define bool unsigned char
#undef YES
#undef NO
#define YES 1
#define NO 0

struct dict *funcs;

struct expr *getexpr(const char *s);
struct token *gettok(char *);
void parse(struct expr *);
struct var *exec(struct func *);
void specfunc(void);

struct var *exec(struct func *f) {
    struct expr *expr;
    struct token *tok;
    struct var *v, *callres, *arg;
    struct func *call;
    char *funcname, *aname;
    unsigned i;
    size_t len;

    i = 0;
    f->scope = dictnew(NULL, NULL);
    while (i < arrcnt(f->args)) {
        aname = arrobj(f->argnms, i);
        v = arrobj(f->args, i++);
        dictadd(f->scope, v, aname);
    }
    
    for (expr = f->expr; expr != NULL; expr = expr->next) {
        
        for (tok = expr->tok; tok != NULL; tok = tok->next) {
            if (strcmp(tok->tok, "def") == 0 && tok->next->type == T_FUNC) {
                expr->type = E_DECL;
                continue;
            }
            else if (tok->type == T_FUNC && expr->type != E_DECL && *(tok->tok) == '(') {
                tok->tok++;
                len = strlen(tok->tok);
                tok->tok[len-1] = 0;
                
                funcname = strtok(tok->tok, " ");
                call = dictobj(funcs, funcname);
                call->args = arrnew(NULL);
                
                funcname++;
                /* funcname becomes the name of the arg here */
                while (funcname != NULL) {
                    arg = dictobj(f->scope, funcname);
                    arradd(call->args, arg);
                    funcname = strtok(NULL, " ");
                }
                
                if (call->type == F_NORM)
                    exec(call);
                else
                    call->spec(call->args);
            }
            else if (strcmp(tok->tok, "def") == 0 && tok->next->type == T_VAR) {
                tok = tok->next;
                v = calloc(1, sizeof(struct var));
                v->name = tok->tok;
                dictadd(f->scope, v, v->name);
                if (tok->next->type == T_ASS) {
                    tok = tok->next->next;
                    if (tok->type == T_LIT) {
                        if (*(tok->tok) == '"') {
                            v->type = V_STR;
                            tok->tok++;
                            tok->tok[strlen(tok->tok) - 1] = 0;
                            v->val.sval = strdup(tok->tok);
                        }
                        else {
                            /* it's a number */
                            v->type = V_INT;
                            v->val.lval = strtol(tok->tok, NULL, 10);
                            
                        }
                    }
                    else if (tok->type == T_FUNC) {
                        tok->tok++;
                        len = strlen(tok->tok);
                        tok->tok[len-1] = 0;
                        
                        funcname = strtok(tok->tok, " ");
                        call = dictobj(funcs, funcname);
                        call->args = arrnew(NULL);
                        
                        /* funcname becomes the name of the arg here */
                        funcname = strtok(NULL, " ");

                        while (funcname != NULL) {
                            arg = dictobj(f->scope, funcname);
                            arradd(call->args, arg);
                            funcname = strtok(NULL, " ");
                        }
                        
                        if (call->type == F_NORM)
                            callres = exec(call);
                        else
                            callres = call->spec(call->args);
                        v->type = callres->type;
                        v->val = callres->val;
                        continue;
                    }
                }
            }
            else if (expr->next && strcmp(expr->next->tok->tok, "end") == 0 && tok->type == T_VAR) {
                return dictobj(f->scope, tok->tok);
            }
            else if (strcmp(tok->tok, "end") != 0) {
                
            }
        }
    }
    
    return NULL;
}

void parse(struct expr *e) {
    struct token *tok;
    struct expr *expr, *fexpr, tmp;
    char *funcnm, *funcargs, *edup;
    struct func *f;
    struct var *arg;
    size_t i;
    
    fexpr = NULL; f = NULL;
    for (expr = e; expr != NULL; expr = expr->next) {
        
        for (tok = expr->tok; tok != NULL; tok = tok->next) {
            if (strcmp(tok->tok, "def") == 0 && tok->next->type == T_FUNC) {
                tok = tok->next;
                edup = strdup(tok->tok);
                funcnm = strtok(edup, "(");
                
                f = calloc(1, sizeof(struct func));
                fexpr = f->expr = expr;
                f->args = arrnew(NULL);
                f->argnms = arrnew(NULL);
                f->name = strdup(funcnm);
                for (funcargs = strtok(NULL, " )"), i = 0; funcargs != NULL; i++) {
                    arg = calloc(1, sizeof(struct var));
                    arg->name = funcargs;
                    arradd(f->args, arg);
                    arradd(f->argnms, strdup(funcargs));
                    
                    funcargs = strtok(NULL, " )");
                }
            }
            else if (strcmp(tok->tok, "end") != 0) {

            }
            if (strcmp(tok->tok, "end") == 0) {
                fexpr = f->expr;
                while (!strstr(fexpr->expr, "end")) {
                    fexpr = fexpr->next;
                }
                tmp.next = fexpr->next;
                fexpr->next = NULL;
                dictadd(funcs, f, f->name);
                expr = &tmp;
            }
        }
    }
}

struct token *gettok(char *s) {
    struct token *tok, *ret;
    int i;
    bool quot, func;
    
    ret = calloc(1, sizeof(struct token));
    
    for (quot = func = NO, i = 0, tok = ret; *s; s++) {
        if (!tok->tok)
            tok->tok = malloc(1), tok->type = T_VAR;
        else
            tok->tok = realloc(tok->tok, (size_t)i+1);
        
        if (*s == '(') {
            func = YES;
            tok->type = T_FUNC;
        }
        else if (*s == ')') {
            func = NO;
        }
        else if (*s == '"') {
            quot = (quot) ? NO : YES;
            tok->type = T_LIT;
        }
        else if (isdigit(*s) && i < 1) {
            tok->type = T_LIT;
        }
        else if (*s == '#')
            return ret;
        
        if (*s != ' ' || ((quot || func) && *s == ' ') || (func && *s == '('))
            tok->tok[i++] = *s;
        else {
            tok->tok[i+1] = 0;
            i = 0;
            if (strcmp(tok->tok, "def") == 0 || strcmp(tok->tok, "end") == 0)
                tok->type = T_KEY;
            else if (*(tok->tok) == '=')
                tok->type = T_ASS;
            else if (*(tok->tok) == ' ' || *(tok->tok) == 0)
                tok->type = T_WHT;
            
            tok->next = calloc(1, sizeof(struct token));
            tok = tok->next;
        }
    }
    return ret;
}

struct expr *getexpr(const char *s) {
    char *cpy, *cpyp, *line;
    struct expr *ex, *ret;
    
    cpy = strdup(s);
    cpyp = cpy;
    
    ret = calloc(1, sizeof(struct expr));
    
    line = strtok(cpy, "\n");
    for (ex = ret; line != NULL; line = strtok(NULL, "\n"), ex->next = calloc(1, sizeof(struct expr)), ex = ex->next) {
        ex->expr = strdup(line);
        ex->tok = gettok(line);
    }
    
    free(cpyp);
    
    return ret;
}

int eval(const char *s, int argc, char **argv) {
    struct expr *ex;
    struct func *entry;
    struct var *v;
    int i;
    struct array *strngs;
    
    strngs = arrnew(NULL);

    funcs = dictnew(NULL, NULL);
    specfunc();
    
    ex = getexpr(s);
    parse(ex);
    
    entry = dictobj(funcs, "entry");

    for (i = 0; i < argc; i++) {
        v = calloc(1, sizeof(struct var));
        v->name = "__arg__";
        v->type = V_STR;
        v->val.sval = argv[i];
        arradd(strngs, v);
    }
    
    v = arrobj(entry->args, 0);
    v->type = V_INT;
    v->val.lval = (long double)argc;
    
    v = arrobj(entry->args, 1);
    v->type = V_ARR;
    v->val.aval = strngs;
    
    exec(entry);
    return 1;
}

void specfunc(void) {
    struct func *f;
    
    f = calloc(1, sizeof(struct func));
    f->name = "+";
    f->type = F_SPEC;
    f->spec = add;
    dictadd(funcs, f, f->name);
    
    f = calloc(1, sizeof(struct func));
    f->name = "print";
    f->type = F_SPEC;
    f->spec = specprnt;
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
    f->name = "idx";
    f->type = F_SPEC;
    f->spec = idx;
    dictadd(funcs, f, f->name);
}