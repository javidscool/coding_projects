//
//  BNROwnedApplicance.m
//  Applicances
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "BNROwnedApplicance.h"

@interface BNROwnedApplicance()
{
    NSMutableSet *_ownerNames;
}
@end

@implementation BNROwnedApplicance

-(instancetype)initWithProductName:(NSString *)pn
                    firstOwnerName:(NSString *)n
{
    //call the superclass's initializer
    if(self = [super initWithProductName:pn]){
        //create a set to hold owners names
        _ownerNames = [[NSMutableSet alloc] init];
        
        //is the first owner name non-nil?
        if(n){
            [_ownerNames addObject:n];
        }
    }
    
    //return a pointer to the new object
    return self;
}

-(instancetype)initWithProductName:(NSString *)pn
{
    return [self initWithProductName:pn firstOwnerName:nil];
}

-(void)addOwnerName:(NSString *)n
{
    [_ownerNames addObject:n];
}
-(void)removeOwnerName:(NSString *)n
{
    [_ownerNames removeObject:n];
}

-(NSSet *)ownerNames
{
    return [_ownerNames copy];
}

@end
