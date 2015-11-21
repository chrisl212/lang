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
struct func *strtofunc(const char *s, struct func *f, struct expr *expr);
struct var *strtolit(const char *s, struct func *f, struct expr *expr);
void standard(void);

struct func *strtofunc(const char *s, struct func *f, struct expr *expr) {
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
                funcarg = strtofunc(funcpy, f, expr);
                if (funcarg->type == F_NORM)
                    arg = exec(funcarg);
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

struct var *exec(struct func *f) {
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
                
                call = strtofunc(tokcp, f, expr);
                
                if (call->type == F_NORM || call->type == F_CTL)
                    callres = exec(call);
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
                    call = strtofunc(tokcp, f, expr);
                    while ((callres = (call->type == F_SPEC)?call->spec(call->args):exec(call))->val.bval) {
                        ctl = calloc(1, sizeof(struct func));
                        ctl->expr = expr->next;
                        ctl->type = F_CTL;
                        ctl->args = arrnew(NULL);
                        ctl->scope = f->scope;
                        exec(ctl);
                        call = strtofunc(tokcp, f, expr);
                    }
                    while (!strstr(expr->expr, "stop"))
                        expr = expr->next;
                }
                else if (strcmp(tok->tok, "if") == 0) {
                    tok = tok->next;
                    tokcp = strdup(tok->tok);
                    tokcp++;
                    tokcp[strlen(tokcp) - 1] = 0;
                    call = strtofunc(tokcp, f, expr);
                    if ((callres = (call->type == F_SPEC)?call->spec(call->args):exec(call))->val.bval) {
                        ctl = calloc(1, sizeof(struct func));
                        ctl->expr = expr->next;
                        ctl->type = F_CTL;
                        ctl->args = arrnew(NULL);
                        ctl->scope = f->scope;
                        exec(ctl);
                        call = strtofunc(tokcp, f, expr);
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
                     
                        call = strtofunc(tokcp, f, expr);
                        
                        if (call->type == F_NORM)
                            callres = exec(call);
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

void parse(struct expr *e) {
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
                fseek(inc, 0, SEEK_END);
                flen = (size_t)ftell(inc);
                fseek(inc, 0, SEEK_SET);
                fconts = malloc(flen + 1);
                fread(fconts, 1, flen, inc);
                fconts[flen] = 0;
                
                ex = getexpr(fconts);
                parse(ex);
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

struct token *gettok(char *s) {
    struct token *tok, *ret;
    int i, nestfunc, nestarr;
    bool quot, func, arr, dict;
    
    ret = calloc(1, sizeof(struct token));
    
    for (quot = dict = arr = func = NO, i = nestfunc = nestarr = 0, tok = ret; *s; s++) {
        if (!tok->tok)
            tok->tok = malloc(1), tok->type = T_VAR;
        else
            tok->tok = realloc(tok->tok, (size_t)i+2);
        
        if (*s == '(') {
            func = YES;
            tok->type = T_FUNC;
            nestfunc++;
        }
        else if (*s == ')') {
            if (--nestfunc == 0)
                func = NO, tok->tok[i++] = *s;
        }
        else if (*s == '"' && !func) {
            quot = (quot) ? NO : YES;
            tok->type = T_LIT;
        }
        else if ((*s == '[' || *s == ']') && !func) {
            tok->type = T_LIT;
            if (*s == ']') {
                if (--nestarr == 0)
                    arr = NO, tok->tok[i++] = *s;
            }
            else if (*s == '[') {
                nestarr++;
                arr = YES;
            }
                
        }
        else if ((*s == '{' || *s == '}') && !func) {
            dict = (dict) ? NO : YES;
            tok->type = T_LIT;
        }
        else if (((isdigit(*s) || (*s == '-' && isdigit(*(s+1))))) && i < 1) {
            tok->type = T_LIT;
            tok->tok[i++] = *s++;
        }
        else if (*s == '#')
            return remwht(ret);
        if (*s == '.' && tok->type == T_LIT && !quot) {
            tok->tok[i++] = *s++;
        }
        
        if (!isalnum(*s) && !quot && !func && !arr && !dict) {
            tok->next = calloc(1, sizeof(struct token));
            if (*s == '=' && tok->type != T_FUNC) {
                tok->next->type = T_ASS;
                tok->next->tok = "=";
                tok->tok[i] = 0;
                tok = tok->next;
                tok->next = calloc(1, sizeof(struct token));
            }
            else
                tok->tok[i] = 0;
            i = 0;
            if (iskey(tok))
                tok->type = T_KEY;
            else if (iswhte(tok))
                tok->type = T_WHT;
            else if (isctrl(tok))
                tok->type = T_CTL;
            
            tok = tok->next;
        }
        else {
            tok->tok[i++] = *s;
        }
    }
    if (iskey(ret))
        ret->type = T_KEY;
    return remwht(ret);
}

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

int eval(const char *s, int argc, char **argv) {
    struct expr *ex;
    struct func *entry;
    struct var *v;
    int i;
    struct array *strngs;
    
    strngs = arrnew(NULL);

    funcs = dictnew(NULL, NULL);
    standard();
    
    ex = getexpr(s);
    parse(ex);
    
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
    
    exec(entry);
    return 1;
}

void standard(void) {
    arith_register(funcs);
    io_register(funcs);
    types_register(funcs);
    condit_register(funcs);
    cpu_register(funcs);
}
