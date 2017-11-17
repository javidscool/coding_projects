//
//  BNRVowelCounting.m
//  VowelCounter
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "BNRVowelCounting.h"

@implementation NSString (BNRVowelCounting)

-(int)bnr_vowelCount
{
    NSCharacterSet *charSet =
    [NSCharacterSet characterSetWithCharactersInString:@"aeiouyAEIOUY"];
    
    NSUInteger count = [self length];
    
    int sum = 0;
    for(int i = 0; i < count; i++){
        unichar c = [self characterAtIndex:i];
        if([charSet characterIsMember:c]){
            sum++;
        }
    }
    
    return sum;
}

@end
