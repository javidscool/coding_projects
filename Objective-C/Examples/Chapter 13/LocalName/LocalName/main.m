//
//  main.m
//  LocalName
//
//  Created by Javid Shamloo on 6/18/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        NSHost *host = [NSHost currentHost];
        
        NSString *myComp = [host localizedName];
        
        NSLog(@"My name is: %@", myComp);
        
    }
    return 0;
}
