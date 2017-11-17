//
//  main.m
//  ZoneChangeBlock
//
//  Created by Javid Shamloo on 6/21/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void(^NotificationBlock)(NSNotification *);

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        NotificationBlock zoneChange = ^(NSNotification *note){
            NSLog(@"The system time zone changed!");
        };
        
        [[NSNotificationCenter defaultCenter]
         addObserverForName:NSSystemTimeZoneDidChangeNotification
         object:nil
         queue:nil
         usingBlock:zoneChange];
        
        [[NSRunLoop currentRunLoop] run];
    }
    
    
    return 0;
}
