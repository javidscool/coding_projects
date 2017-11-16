//
//  main.m
//  HowLongIveBeenAlive
//
//  Created by Javid Shamloo on 6/18/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        NSDateComponents *comps = [[NSDateComponents alloc] init];
        [comps setYear: 1986];
        [comps setMonth: 1];
        [comps setDay: 16];
        [comps setHour: 9];
        [comps setMinute: 14];
        [comps setSecond: 0];
        
        NSCalendar *g = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar];
        NSDate *dateOfBirth = [g dateFromComponents:comps];
        
        NSDate *now = [[NSDate alloc] init];
        
        double secondsSinceIWasBorn = [now timeIntervalSinceDate:dateOfBirth];
        
        NSLog(@"It has been %f seconds since I was born.", secondsSinceIWasBorn);
        
    }
    return 0;
}
