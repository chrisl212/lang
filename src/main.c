/*
//  main.c
//  pewter_lang
//
//  Created by Christopher Loonam on 11/16/15.
//  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdio.h>
#include "parser/eval.h"

int main(int argc, char **argv) {
    FILE *f;
    size_t n;
    char buf[1024];
    
    f = fopen(argv[1], "r");
    if (!f) {
        f = stdin;
    }
    n = fread(buf, 1, 1024, f);
    buf[n] = 0;
    
    eval(buf, argc, argv);
    
    fclose(f);
}
