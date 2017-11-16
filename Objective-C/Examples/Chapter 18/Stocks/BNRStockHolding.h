//
//  BNRStockHolding.h
//  Stocks
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BNRStockHolding : NSObject

{
    float _purchaseSharePrice;
    float _currentSharePrice;
    int _numberOfShares;
}

@property (nonatomic, copy) NSString *symbol;

-(float)purchaseSharePrice;
-(void)setPurchaseSharePrice:(float)p;

-(float)currentSharePrice;
-(void)setCurrentSharePrice:(float)c;

-(int)numberOfShares;
-(void)setNumberOfShares:(int)n;


-(float)costInDollars;
-(float)valueInDollars;
@end
