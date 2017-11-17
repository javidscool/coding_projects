//
//  BNRAsset.m
//  BMITime
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "BNRAsset.h"
#import "BNREmployee.h"

@implementation BNRAsset

-(NSString *)description
{    
    //is holder non-nil
    if(self.holder){
        return [NSString stringWithFormat:@"<%@: $%d, assigned to %@>",
                self.label, self.resaleValue, self.holder];
    }
    else{
        return [NSString stringWithFormat:@"<%@: $%d unassigned>", self.label, self.resaleValue];
    }
}

-(void)dealloc
{
    NSLog(@"deallocating %@", self);
}

@end
