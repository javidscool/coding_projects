//
//  main.m
//  Stocks
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BNRForeignStockHolding.h"
#import "BNRPortfolio.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
    
        //create three BNRStockHoldings
        BNRStockHolding *stock1 = [[BNRStockHolding alloc] init];
        [stock1 setPurchaseSharePrice:2.30];
        [stock1 setCurrentSharePrice:4.50];
        [stock1 setNumberOfShares:40];
        stock1.symbol = @"XYZ";
        
        BNRStockHolding *stock2 = [[BNRStockHolding alloc] init];
        [stock2 setPurchaseSharePrice:12.19];
        [stock2 setCurrentSharePrice:10.56];
        [stock2 setNumberOfShares:90];
        stock2.symbol = @"ABC";
        
        BNRStockHolding *stock3 = [[BNRStockHolding alloc] init];
        [stock3 setPurchaseSharePrice:45.10];
        [stock3 setCurrentSharePrice:49.51];
        [stock3 setNumberOfShares:210];
        stock3.symbol = @"LMN";
        
        BNRForeignStockHolding *stock4 = [[BNRForeignStockHolding alloc] init];
        [stock4 setPurchaseSharePrice:2.30];
        [stock4 setCurrentSharePrice:4.50];
        [stock4 setNumberOfShares:40];
        [stock4 setConversionRate:0.94];
        stock4.symbol = @"JNS";
        
        BNRForeignStockHolding *stock5 = [[BNRForeignStockHolding alloc] init];
        [stock5 setPurchaseSharePrice:25.10];
        [stock5 setCurrentSharePrice:29.51];
        [stock5 setNumberOfShares:110];
        [stock5 setConversionRate:0.84];
        stock5.symbol = @"OPP";
        
        NSMutableArray *stockList = [NSMutableArray array];
        
        [stockList addObject: stock1];
        [stockList addObject: stock2];
        [stockList addObject: stock3];
        [stockList addObject: stock4];
        [stockList addObject: stock5];
        
        for(BNRStockHolding *s in stockList){
            NSLog(@"The cost in dollars is %.2f and the value in dollars is %.2f",
                  [s costInDollars], [s valueInDollars]);
        }
        
        BNRPortfolio *portfolio = [[BNRPortfolio alloc] init];
        [portfolio setHoldings:stockList];
        
        NSLog(@"Holdings: %@", portfolio);
        
        NSLog(@"Top values holdings: %@", [portfolio topValuedHoldings]);
        NSLog(@"Stock holding ordered: %@", [portfolio stockHoldingOrdered]);
        
        [portfolio removeAsset:stock1];
        NSLog(@"Holdings: %@", portfolio);
        
        [portfolio removeAsset:stock5];
        NSLog(@"Holdings: %@", portfolio);
    }
    return 0;
}
