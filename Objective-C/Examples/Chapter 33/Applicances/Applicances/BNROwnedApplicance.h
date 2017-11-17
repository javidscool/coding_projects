//
//  BNROwnedApplicance.h
//  Applicances
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BNRAppliance.h"

@interface BNROwnedApplicance : BNRAppliance
@property (readonly) NSSet *ownerNames;

//the designated initializer
-(instancetype)initWithProductName:(NSString *)pn
                    firstOwnerName:(NSString *)n;

-(instancetype)initWithProductName:(NSString *)pn;

-(void)addOwnerName:(NSString *)n;
-(void)removeOwnerName:(NSString *)n;

@end
