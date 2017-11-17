//
//  BNRObserver.m
//  Callbacks
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "BNRObserver.h"

@implementation BNRObserver

-(void)observeValueForKeyPath:(NSString *)keyPath
                     ofObject:(id)object
                       change:(NSDictionary *)change
                      context:(void *)context
{

        NSString *oldValue = [change
                              objectForKey:NSKeyValueChangeOldKey];
        NSString *newValue = [change
                              objectForKey:NSKeyValueChangeNewKey];
        NSLog(@"Observed: %@ of %@ was changed from %@ to %@",
              keyPath, object, oldValue, newValue);
}

@end
