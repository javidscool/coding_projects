//
//  BNRPortfolio.m
//  Stocks
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "BNRPortfolio.h"
#import "BNRStockHolding.h"

@interface BNRPortfolio()

{
    NSMutableArray *_holdings;
}

@end

@implementation BNRPortfolio

//accessors for holdings property
-(void)setHoldings:(NSArray *)a
{
    _holdings = [a mutableCopy];
}

-(NSArray *)holdings
{
    return [_holdings copy];
}

-(void)addAsset:(BNRStockHolding *)a
{
    if(!_holdings){
        _holdings = [[NSMutableArray alloc] init];
    }
    
    [_holdings addObject:a];
}

-(void)removeAsset:(BNRStockHolding *)a
{
    for (BNRStockHolding *stock in _holdings) {
        if (stock.symbol == a.symbol) {
            [_holdings removeObject:a];
            break;
        }
    }
}

-(float)totalValue
{
    float sum = 0.0;
    for(BNRStockHolding *a in _holdings){
        sum += [a valueInDollars];
    }
    
    return sum;
}

-(NSArray *)topValuedHoldings{
    NSSortDescriptor *valInDoll = [NSSortDescriptor
                                   sortDescriptorWithKey:@"valueInDollars"
                                   ascending:NO];
    
    [_holdings sortUsingDescriptors:@[valInDoll]];
    
    NSArray *topValues = @[_holdings[0], _holdings[1], _holdings[2]];
    
    return topValues;
}

-(NSArray *)stockHoldingOrdered{
    
    NSSortDescriptor *stockName = [NSSortDescriptor
                                   sortDescriptorWithKey:@"symbol"
                                   ascending:YES];
    
    [_holdings sortUsingDescriptors:@[stockName]];
    
    NSArray *result = self.holdings;
    
    return result;
    
}

-(NSString *)description
{
    return [NSString stringWithFormat:@"%@", self.holdings];
}

@end
