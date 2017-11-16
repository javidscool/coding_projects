//
//  main.m
//  Groceries
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        //create three NSDate objects
        NSString *grocery1 = @"Eggs";
        NSString *grocery2 = @"Cheese";
        NSString *grocery3 = @"Milk";
        
        //create an empty mutable array
        NSMutableArray *groceryList = [NSMutableArray array];
        
        //add two date objects to the array
        [groceryList addObject:grocery2];
        [groceryList addObject:grocery3];
        
        //add yesterday to the beginning of the list
        [groceryList insertObject:grocery1 atIndex:0];
        
        NSLog(@"My grocery list is:");
        //iterate over the array
        for(NSDate *d in groceryList){
            NSLog(@"%@", d);
        }
    }
    return 0;
}
