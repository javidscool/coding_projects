//
//  BNRLogger.m
//  Callbacks
//
//  Created by Javid Shamloo on 6/20/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "BNRLogger.h"

@interface BNRLogger()
-(void)zoneChange:(NSNotification *)note;
@end

@implementation BNRLogger

-(void)zoneChange:(NSNotification *)note
{
    NSLog(@"The system time zone changed!");
}

-(NSString *)lastTimeString
{
    static NSDateFormatter *dateFormatter = nil;
    if(!dateFormatter){
        dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setTimeStyle:NSDateFormatterMediumStyle];
        [dateFormatter setDateStyle:NSDateFormatterMediumStyle];
    }
    return [dateFormatter stringFromDate:self.lastTime];
}

-(void)updateLastTime:(NSTimer *)t
{
    NSDate *now = [NSDate date];
    //[self setLastTime:now];
    [self willChangeValueForKey:@"lastTime"];
    _lastTime = now;
    [self didChangeValueForKey:@"lastTime"];
    NSLog(@"Just set time to %@", self.lastTimeString);
}

+(NSSet *)keyPathsForValuesAffectingLastTimeString
{
    return [NSSet setWithObject:@"lastTime"];
}

//called each time a chunk of data arrives
-(void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    NSLog(@"recieved %lu bytes", [data length]);
    
    //create a mutable data if it does not already exist
    if(!_incomingData){
        _incomingData = [[NSMutableData alloc] init];
    }
    
    [_incomingData appendData:data];
}

//called when the last chunk has been processed
-(void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    NSLog(@"Got is all!");
    NSString *string = [[NSString alloc] initWithData:_incomingData
                                        encoding:NSUTF8StringEncoding];
    
    _incomingData = nil;
    NSLog(@"string has %lu characters", [string length]);
    
    NSLog(@"The whole string is %@", string);
}

//called if the fetch fails
-(void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    NSLog(@"connection falied: %@", [error localizedDescription]);
    _incomingData = nil;
}

@end
