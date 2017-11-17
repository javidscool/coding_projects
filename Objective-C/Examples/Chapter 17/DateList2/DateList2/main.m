//
//  main.m
//  DateList2
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
        
        //create an empty mutable array
        NSMutableArray *dateList = [NSMutableArray array];
        
        //add two date objects to the array
        [dateList addObject:now];
        [dateList addObject:tomorrow];
        
        //add yesterday to the beginning of the list
        [dateList insertObject:yesterday atIndex:0];
        
        //iterate over the array
        for(NSDate *d in dateList){
            NSLog(@"Here is a date: %@", d);
        }
        
        //remove yesterday
        [dateList removeObjectAtIndex:0];
        NSLog(@"Now the first date is %@", dateList[0]);
        
    }
    return 0;
}
