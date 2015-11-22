/*
 expr.c
 pewter_lang
 
 Created by Christopher Loonam on 11/22/15.
 Copyright (c) 2015 Christopher Loonam. All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "../foundation/foundation.h"

struct expr *getexpr(const char *s) {
    char *cpy, *cpyp, *line;
    struct expr *ex, *ret;
    
    cpy = strdup(s);
    cpyp = cpy;
    
    ret = calloc(1, sizeof(struct expr));
    
    line = strtok(cpy, "\n;");
    for (ex = ret; line != NULL; line = strtok(NULL, "\n;"), ex->next = calloc(1, sizeof(struct expr)), ex = ex->next) {
        ex->expr = strdup(line);
        ex->tok = gettok(line);
        ex->tok = ex->tok;
    }
    
    free(cpyp);
    
    return ret;
}

void parse(struct expr *e, struct dict *funcs) {
    struct token *tok;
    struct expr *expr, *fexpr, tmp, *ex;
    char *funcnm, *funcargs, *edup, *include, *fconts;
    struct func *f;
    struct var *arg;
    size_t i, flen;
    FILE *inc;
    
    fexpr = NULL; f = NULL;
    for (expr = e; expr != NULL; expr = expr->next) {
        
        for (tok = expr->tok; tok != NULL; tok = tok->next) {
            if (!tok->tok)
                break;
            if (strcmp(tok->tok, "func") == 0 && tok->next && tok->next->type == T_FUNC) {
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
            else if (strcmp(tok->tok, "using") == 0) {
                tok = tok->next;
                include = malloc(strlen(tok->tok) + 5);
                strcpy(include, tok->tok);
                strcat(include, ".prg");
                inc = fopen(include, "r");
                if (!inc) {
                    include = malloc(strlen("/usr/local/include/pewter/") + strlen(tok->tok) + 5);
                    strcpy(include, "/usr/local/include/pewter/");
                    strcat(include, tok->tok);
                    strcat(include, ".prg");
                    inc = fopen(include, "r");
                }
                
                fseek(inc, 0, SEEK_END);
                flen = (size_t)ftell(inc);
                fseek(inc, 0, SEEK_SET);
                fconts = malloc(flen + 1);
                fread(fconts, 1, flen, inc);
                fconts[flen] = 0;
                
                ex = getexpr(fconts);
                parse(ex, funcs);
            }
            if (strcmp(tok->tok, "end") == 0) {
                if (!fexpr)
                    break;
                fexpr = f->expr;
                while (!strstr(fexpr->expr, "end")) {
                    fexpr = fexpr->next;
                }
                tmp.next = fexpr->next;
                fexpr->next = NULL;
                expr = &tmp;
                dictadd(funcs, f, f->name);
            }
        }
    }
}
