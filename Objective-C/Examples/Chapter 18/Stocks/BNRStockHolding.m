//
//  BNRStockHolding.m
//  Stocks
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "BNRStockHolding.h"

@implementation BNRStockHolding

-(float)purchaseSharePrice
{
    return _purchaseSharePrice;
}
-(void)setPurchaseSharePrice:(float)p
{
    _purchaseSharePrice = p;
}

-(float)currentSharePrice
{
    return _currentSharePrice;
}
-(void)setCurrentSharePrice:(float)c
{
    _currentSharePrice = c;
}

-(int)numberOfShares
{
    return _numberOfShares;
}
-(void)setNumberOfShares:(int)n
{
    _numberOfShares = n;
}

-(float)costInDollars
{
    return _purchaseSharePrice * _numberOfShares;
}

-(float)valueInDollars
{
    return _currentSharePrice * _numberOfShares;
}

-(NSString *)description
{
    return [NSString stringWithFormat:@"I am a stock holder with %d shares and %.2f value and a symbol of %@",
        self.numberOfShares, self.valueInDollars, self.symbol];
}

@end
