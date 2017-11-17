//
//  BNREmployee.h
//  BMITime
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BNRPerson.h"
@class BNRAsset;

@interface BNREmployee : BNRPerson

@property (nonatomic) unsigned int employeeID;
@property (nonatomic) NSDate *hireDate;

@property (nonatomic, copy) NSSet *assets;

-(double)yearsOfEmployment;

-(void)addAsset:(BNRAsset *)a;
-(unsigned int)valueOfAssets;

@end
