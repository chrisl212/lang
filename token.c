/*
  token.c
  pewter_lang

  Created by Christopher Loonam on 11/18/15.
  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "token.h"

struct token *duptok(struct token *);

unsigned char iskey(struct token *tok) {
    if (strcmp(tok->tok, "func") == 0 || strcmp(tok->tok, "end") == 0 || strcmp(tok->tok, "stop") == 0)
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
    
    return ret;
}

struct token *remwht(struct token *tok) {
    struct token *ret, *cpy;
    
    cpy = duptok(tok);
    ret = cpy;
    tok = tok->next;
    while (tok) {
        if (tok->type != T_WHT && tok->tok)
            cpy->next = duptok(tok), cpy = cpy->next;
        tok = tok->next;
    }
    
    return ret;
}
