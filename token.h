/*
//  token.h
//  pewter_lang
//
//  Created by Christopher Loonam on 11/16/15.
//  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#ifndef pewter_lang_token_h
#define pewter_lang_token_h

enum tok_type {
    T_VAR = 1,
    T_FUNC,
    T_KEY,
    T_LIT,
    T_ASS,
    T_WHT
};

struct token {
    char *tok;
    enum tok_type type;
    struct token *next;
};

#endif
