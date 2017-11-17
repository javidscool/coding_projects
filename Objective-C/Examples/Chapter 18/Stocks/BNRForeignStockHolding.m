//
//  BNRForeignStockHolding.m
//  Stocks
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "BNRForeignStockHolding.h"

@implementation BNRForeignStockHolding

-(float)costInDollars
{
    return _purchaseSharePrice * _numberOfShares * self.conversionRate;
}

-(float)valueInDollars
{
    return _currentSharePrice * _numberOfShares * self.conversionRate;
}

-(NSString *)description
{
    return [NSString stringWithFormat:@"I am a foreign stock holder with %d shares and %.2f value and a symbol of %@",self.numberOfShares, self.valueInDollars, self.symbol];
}

@end
