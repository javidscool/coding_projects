//
//  main.m
//  Applicances
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BNRAppliance.h"
#import "BNROwnedApplicance.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        BNRAppliance *a = [[BNRAppliance alloc] init];
        NSLog(@"a is %@", a);
        
        [a setProductName:@"Washing Machine"];
        [a setVoltage:240];
        NSLog(@"a is %@", a);
        
        BNROwnedApplicance *b = [[BNROwnedApplicance alloc] init];
        NSLog(@"b is %@", b);
        b = [[BNROwnedApplicance alloc] initWithProductName:@"Toaster"];
        NSLog(@"b is %@", b);
        b = [[BNROwnedApplicance alloc] initWithProductName:@"Toaster"
                                             firstOwnerName:@"Javid"];
        NSLog(@"owners names are is %@", b.ownerNames);
    }
    return 0;
}
