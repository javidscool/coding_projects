//
//  main.m
//  CoundownObjC
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <stdlib.h>
#include <readline/readline.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        NSLog(@"Where should I start counting?\n");
        
        const char *countFrom = readline(NULL);
        
        NSString *myString = [NSString stringWithUTF8String:countFrom];
        NSInteger myInt = [myString integerValue];
        
        //int counter = atoi(countFrom);
        
        for(NSInteger i = myInt; i >= 0; i -= 3){
            
            NSLog(@"%ld\n", (long)i);
            
            if(i % 5 == 0){
                NSLog(@"Found one!\n");
            }
        }
    }
    return 0;
}
