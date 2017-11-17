//
//  main.m
//  VowelMovement
//
//  Created by Javid Shamloo on 6/21/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void(^ArrayEnumerationBlock)(id, NSUInteger, BOOL *);

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        //create an array of strings and a container for devowelized ones
        NSArray *originalStrings = @[@"Sauerkraut", @"Raygun",
                                     @"Big Nerd Ranch", @"Mississippi"];
        NSLog(@"original strings: %@", originalStrings);
        
        NSMutableArray *devowelizedStrings = [NSMutableArray array];
        
        //create a list of characters to be removed from the string
        NSArray *vowels = @[@"a", @"e", @"i", @"o", @"u"];
        
        //declare the block variable
        ArrayEnumerationBlock devowelizer;
        
        devowelizer = ^(id string, NSUInteger i, BOOL *stop){
            
            NSRange yRange = [string rangeOfString:@"y"
                                           options:NSCaseInsensitiveSearch];
            //did I find a y?
            if(yRange.location != NSNotFound){
                *stop = YES; //prevent further iterations
                return; //end this iteration
            }
            
            
            NSMutableString *newString = [NSMutableString
                                          stringWithString:string];
            //iterate over the array of vowels, replacing occurrences of each with
            //an empty string
            for(NSString *s in vowels){
                //make a range to go thru
                NSRange fullRange = NSMakeRange(0, [newString length]);
                
                [newString replaceOccurrencesOfString:s
                                           withString:@""
                                              options:NSCaseInsensitiveSearch
                                                range:fullRange];
            }
            
            [devowelizedStrings addObject:newString];
        }; //end of block assignment
        
        //[originalStrings enumerateObjectsUsingBlock:devowelizer];
        
        //using anonymous block
        [originalStrings enumerateObjectsUsingBlock:
         ^(id string, NSUInteger i, BOOL *stop){
             
             NSRange yRange = [string rangeOfString:@"y"
                                            options:NSCaseInsensitiveSearch];
             //did I find a y?
             if(yRange.location != NSNotFound){
                 *stop = YES; //prevent further iterations
                 return; //end this iteration
             }
             
             
             NSMutableString *newString = [NSMutableString
                                           stringWithString:string];
             //iterate over the array of vowels, replacing occurrences of each with
             //an empty string
             for(NSString *s in vowels){
                 //make a range to go thru
                 NSRange fullRange = NSMakeRange(0, [newString length]);
                 
                 [newString replaceOccurrencesOfString:s
                                            withString:@""
                                               options:NSCaseInsensitiveSearch
                                                 range:fullRange];
             }
             
             [devowelizedStrings addObject:newString];
         }]; //end of block assignment
        NSLog(@"devowelized strings: %@", devowelizedStrings);
        
    }
    return 0;
}
