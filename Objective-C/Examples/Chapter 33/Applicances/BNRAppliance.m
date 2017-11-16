//
//  BNRAppliance.m
//  Applicances
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "BNRAppliance.h"

@implementation BNRAppliance

-(instancetype)init
{
    return [self initWithProductName:@"Unknown"];
}

-(instancetype)initWithProductName:(NSString *)pn
{
    //call NSObject's init method and did it return something non-nil?
    if(self = [super init]){
        //set the product name
        _productName = [pn copy];
        
        //give voltage a starting value
        _voltage = 120;
    }
    
    return self;
}

-(NSString *)description
{
    return [NSString stringWithFormat:@"<%@: %d volts>",
            self.productName, self.voltage];
}

@end
