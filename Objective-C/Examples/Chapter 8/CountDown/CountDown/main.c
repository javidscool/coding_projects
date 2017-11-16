//
//  main.c
//  CountDown
//
//  Created by Javid Shamloo on 6/18/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

int main(int argc, const char * argv[]) {
    
    printf("Where should I start counting?\n");
    const char *countFrom = readline(NULL);
    int counter = atoi(countFrom);
    
    for(int i = counter; i >= 0; i -= 3){
        
        printf("%d\n", i);
        
        if(i % 5 == 0){
            printf("Found one!\n");
        }
        
    }
    
    printf("An int is %zu bytes\n", sizeof(&counter));
    
    return 0;
}
