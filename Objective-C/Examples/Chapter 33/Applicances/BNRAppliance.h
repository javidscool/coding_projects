//
//  BNRAppliance.h
//  Applicances
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BNRAppliance : NSObject

@property (nonatomic, copy) NSString *productName;
@property (nonatomic) int voltage;

//the designated initializer
-(instancetype)initWithProductName:(NSString *)pn;

-(NSString *)description;

@end
