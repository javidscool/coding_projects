//
//  BNRDateConvenience.h
//  DateMonger
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSDate (BNRDateConvenience)
-(NSDate *)bnr_midniteofYear:(int)year
                        Month:(int)month
                        Day:(int)day;
@end
