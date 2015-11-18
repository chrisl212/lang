/*
//  main.c
//  pewter_lang
//
//  Created by Christopher Loonam on 11/16/15.
//  Copyright (c) 2015 Christopher Loonam. All rights reserved.
*/

#include <stdio.h>
#include "eval.h"

int main(int argc, char **argv) {
    FILE *f;
    size_t n;
    char buf[1024];
    
    f = fopen("/Users/christopherloonam/Projects/Small Projects/pewter_lang/pewter_lang/test.prg", "r");
    n = fread(buf, 1, 1024, f);
    buf[n] = 0;
    
    eval(buf, argc, argv);
    
    fclose(f);
}
