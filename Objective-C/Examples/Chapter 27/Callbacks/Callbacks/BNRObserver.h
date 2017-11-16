//
//  BNRObserver.h
//  Callbacks
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BNRObserver : NSObject

-(void)observeValueForKeyPath:(NSString *)keyPath
                ofObject:(id)object
                change:(NSDictionary *)change
                context:(void *)context;
@end
