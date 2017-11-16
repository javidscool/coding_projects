//
//  main.m
//  DateMonger
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BNRDateConvenience.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        NSDate *myDate = [[NSDate alloc] init];
        NSLog(@"The date January 16, 1986 at midnite is %@:", [myDate bnr_midniteofYear:1986
                                                                                  Month:1
                                                                                    Day:16]);
    }
    return 0;
}
