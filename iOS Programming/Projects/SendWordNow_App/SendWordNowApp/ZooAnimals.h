//
//  ZooAnimals.h
//  SendWordNowApp
//
//  Created by Javid Shamloo on 4/10/17.
//  Copyright © 2017 SendWordNow. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ZooAnimals : NSObject

+(instancetype)sharedZoo;

@property (nonatomic, strong) NSIndexPath *indexPath;

@property (nonatomic, readonly, copy) NSDictionary *allAnimals;

@end
