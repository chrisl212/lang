/*
 var.c
 pewter_lang
 
 Created by Christopher Loonam on 11/22/15.
 Copyright (c) 2015 Christopher Loonam. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "../foundation/foundation.h"

struct var *strtolit(const char *s, struct func *f, struct expr *expr) {
    struct var *arg, *d1, *d2;
    struct token *tok;
    array_t *arr;
    dict_t *dict;
    char *funcname;
    
    d1 = d2 = NULL;
    funcname = strdup(s);
    arg = calloc(1, sizeof(struct var));
    
    if (*funcname == '"') {
        arg->type = V_STR;
        funcname++;
        funcname[strlen(funcname)] = 0;
        arg->val.sval = funcname;
        arg->name = funcname;
    }
    /* number literal */
    else if ((isdigit(*s) || (*s == '-' && isdigit(*(s+1))))) {
        if (!strstr(s, ".")) {
            arg->type = V_INT;
            if (*funcname == '0')
                arg->val.ival = strtol(funcname, NULL, 2);
            else
                arg->val.ival = strtol(funcname, NULL, 10);
            arg->name = funcname;
        }
        else {
            arg->type = V_DOUB;
            arg->val.fval = strtold(funcname, NULL);
            arg->name = funcname;
        }
    }
    else if (*funcname == '[') {
        funcname++;
        funcname[strlen(funcname) - 1] = 0;
        arg->type = V_ARR;
        
        while (isspace(*funcname)) funcname++;
        if (strlen(funcname) < 1) {
            arg->val.aval = arrnew(NULL);
            return arg;
        }
        
        tok = gettok(funcname);
        
        arg->val.aval = arrnew(NULL);
        arr = arg->val.aval;
        while (tok) {
            if (tok->type == T_VAR) {
                arradd(arr, dictobj(f->scope, tok->tok));
            }
            else if (tok->type == T_LIT) {
                arradd(arr, strtolit(tok->tok, f, expr));
            }
            tok = tok->next;
        }
    }
    else if (*funcname == '{') {
        funcname++;
        funcname[strlen(funcname) - 1] = 0;
        arg->type = V_DIC;
        
        while (isspace(*funcname)) funcname++;
        if (strlen(funcname) < 1) {
            arg->val.dval = dictnew(NULL, NULL);
            return arg;
        }
        
        tok = gettok(funcname);
        
        arg->val.aval = arrnew(NULL);
        dict = arg->val.dval;
        while (tok) {
            if (tok->type == T_VAR) {
                if (!d1)
                    d1 = dictobj(f->scope, tok->tok);
                else if (!d2)
                    d2 = dictobj(f->scope, tok->tok);
            }
            else if (tok->type == T_LIT) {
                if (!d1)
                    d1 = strtolit(tok->tok, f, expr);
                else if (!d2)
                    d2 = strtolit(tok->tok, f, expr);
            }
            if (d1 && d2) {
                dictadd(dict, d2, d1->val.sval);
                d1 = d2 = NULL;
            }
            tok = tok->next;
        }
    }
    return arg;
}
