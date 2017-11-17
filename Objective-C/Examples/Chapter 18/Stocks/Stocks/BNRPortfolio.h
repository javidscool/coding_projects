//
//  BNRPortfolio.h
//  Stocks
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>
@class BNRStockHolding;

@interface BNRPortfolio : NSObject

@property (nonatomic, copy) NSArray *holdings;

-(void)addAsset:(BNRStockHolding *)a;
-(void)removeAsset:(BNRStockHolding *)a;
-(float)totalValue;
-(NSArray *)topValuedHoldings;
-(NSArray *)stockHoldingOrdered;

@end
