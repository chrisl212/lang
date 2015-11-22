/*
 token.c
 pewter_lang
 
 Created by Christopher Loonam on 11/18/15.
 Copyright (c) 2015 Christopher Loonam. All rights reserved.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "token.h"

#undef bool
#define bool unsigned char
#undef YES
#undef NO
#define YES 1
#define NO 0

struct token *duptok(struct token *);

unsigned char iskey(struct token *tok) {
    if (strcmp(tok->tok, "func") == 0 || strcmp(tok->tok, "end") == 0 || strcmp(tok->tok, "stop") == 0 || strcmp(tok->tok, "using") == 0)
        return 1;
    return 0;
}

unsigned char iswhte(struct token *tok) {
    if (*(tok->tok) == ' ' || *(tok->tok) == 0)
        return 1;
    return 0;
}

unsigned char isctrl(struct token *tok) {
    if (strcmp(tok->tok, "if") == 0 || strcmp(tok->tok, "while") == 0)
        return 1;
    return 0;
}

struct token *duptok(struct token *tok) {
    struct token *ret;
    
    ret = calloc(1, sizeof(struct token));
    ret->tok = strdup(tok->tok);
    ret->type = tok->type;
    ret->next = NULL;
    
    return ret;
}

struct token *remwht(struct token *tok) {
    struct token *ret, *cpy;
    
    cpy = duptok(tok);
    ret = cpy;
    tok = tok->next;
    while (tok) {
        if (tok->type != T_WHT && tok->tok && tok->tok[0])
            cpy->next = duptok(tok), cpy = cpy->next;
        tok = tok->next;
    }
    
    return ret;
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
