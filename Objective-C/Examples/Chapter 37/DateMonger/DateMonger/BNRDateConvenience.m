//
//  BNRDateConvenience.m
//  DateMonger
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "BNRDateConvenience.h"

@implementation NSDate (BNRDateConvenience)

-(NSDate *)bnr_midniteofYear:(int)year
                       Month:(int)month
                         Day:(int)day
{
    NSDateComponents *comps = [[NSDateComponents alloc] init];
    [comps setYear:year];
    [comps setMonth:month];
    [comps setDay:day];
    [comps setHour:-5];
    [comps setMinute:0];
    [comps setSecond:0];
    
    NSCalendar *g = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar];
    NSDate *date = [g dateFromComponents:comps];
    
    return date;
}

@end
