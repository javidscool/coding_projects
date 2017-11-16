//
//  BNRLogger.h
//  Callbacks
//
//  Created by Javid Shamloo on 6/20/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BNRLogger : NSObject
    <NSURLConnectionDataDelegate, NSURLConnectionDelegate>

{
    NSMutableData *_incomingData;
}

@property (nonatomic) NSDate *lastTime;
-(NSString *)lastTimeString;
-(void)updateLastTime:(NSTimer *)t;
-(void)zoneChange:(NSNotification *)note;

//methods for delegates
-(void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data;
-(void)connectionDidFinishLoading:(NSURLConnection *)connection;
-(void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error;

@end
