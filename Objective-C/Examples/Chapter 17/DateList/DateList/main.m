//
//  main.m
//  DateList
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        //create three NSDate objects
        NSDate *now = [NSDate date];
        NSDate *tomorrow = [now dateByAddingTimeInterval:24.0 * 60.0 * 60.0];
        NSDate *yesterday = [now dateByAddingTimeInterval:-24.0 * 60.0 * 60.0];
        
        //create an array containing all three
        NSArray *dateList = @[now, tomorrow, yesterday];
        
        //print a couple of dates
        NSLog(@"The first date is %@", dateList[0]);
        NSLog(@"The third date is %@", dateList[2]);
        
        //how many dates are in the array?
        NSLog(@"There are %lu dates", [dateList count]);
        
        //iterate over the array
        for(NSDate *d in dateList){
            NSLog(@"Here is a date: %@", d);
        }
    }
    return 0;
}
