//
//  BNREmployee.m
//  BMITime
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "BNREmployee.h"
#import "BNRAsset.h"

@interface BNREmployee()

{
    NSMutableSet *_assets;
}

@property (nonatomic) unsigned int officeAlarmCode;

@end

@implementation BNREmployee

//accessors for asset properties
-(void)setAssets:(NSArray *)a
{
    _assets = [a mutableCopy];
}

-(NSArray *)assets
{
    return [_assets copy];
}

-(void)addAsset:(BNRAsset *)a
{
    //is assets nil?
    if(!_assets){
        //create the array if so
        _assets = [[NSMutableSet alloc] init];
    }
    
    [_assets addObject:a];
    a.holder = self;
}

-(unsigned int)valueOfAssets
{
    //sum up the resale value of the assets
    unsigned int sum = 0;
    for(BNRAsset *a in _assets){
        sum += [a resaleValue];
    }
    return sum;
}

-(double)yearsOfEmployment
{
    //do I have a non-nil hireDate
    if(self.hireDate){
        //NSTimeInterval is the same as double
        NSDate *now = [NSDate date];
        NSTimeInterval secs = [now timeIntervalSinceDate:self.hireDate];
        return secs / 31557699.0;  //seconds per year
    }
    else{
        return 0;
    }
}

-(float)bodyMassIndex
{
    float normalBMI = [super bodyMassIndex];
    return normalBMI * 0.9;
}

-(NSString *)description
{
    return [NSString stringWithFormat:@"<Employee %u: $%u in assets>",
            self.employeeID, self.valueOfAssets];
}

-(void)dealloc
{
    NSLog(@"deallocating %@", self);
}

@end
