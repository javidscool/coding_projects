//
//  ZooAnimals.m
//  SendWordNowApp
//
//  Created by Javid Shamloo on 4/10/17.
//  Copyright © 2017 SendWordNow. All rights reserved.
//

#import "ZooAnimals.h"

@interface ZooAnimals()

@property (nonatomic, strong) NSDictionary *zoo;

@end


@implementation ZooAnimals

+(instancetype)sharedZoo{
    static ZooAnimals *singleton;
    
    //dispatch_once makes sure this code is run exactly once
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        singleton = [[self alloc] initPrivate];
    });
    
    return singleton;
}

//if a programmer calls [[ZooAnimals alloc] init], let him know the error
-(instancetype)init
{
    [NSException raise:@"Singleton" format:@"Use +[ZooAnimals sharedZoo]"];
    
    return nil;
}

//here is the real (secret) initializer
-(instancetype)initPrivate{
    self = [super init];
    if(self){
        
        //load in the zoo animals
        _zoo = @{ @"Mammals" : @[ @[@"cat", @1], @[@"dog", @2], @[@"bear", @3] ],
                  @"Birds" : @[ @[@"ostrich", @1], @[@"robin", @2], @[@"sparrow", @3], @[@"parrot", @4] ],
                 @"Fish" : @[ @[@"goldfish", @1], @[@"bass", @2], @[@"shark", @3] ]
                 };
        
    }
    
    return self;
}

-(NSDictionary *)allAnimals
{
    return [self.zoo copy];
}

@end
