//
//  main.m
//  BMITime
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BNREmployee.h"
#import "BNRAsset.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        /*
        //create an instance of BNREmployee
        BNREmployee *mikey = [[BNREmployee alloc] init];
        
        //give the instance variables interesting values using setters
        mikey.weightInKilos = 96;
        mikey.heightInMeters = 1.8;
        mikey.employeeID = 12;
        mikey.hireDate = [NSDate dateWithNaturalLanguageString:@"Aug 2nd, 2010"];
        
        //log the instance variables using getters
        float height = mikey.heightInMeters;
        int weight = mikey.weightInKilos;
        NSLog(@"mikey is %.2f meters tall and weighs %d kilograms", height, weight);
        NSDate *date = mikey.hireDate;
        NSLog(@"%@ hired on %@", mikey, date);
        
        //log some values using custom methods
        float bmi = [mikey bodyMassIndex];
        double years = [mikey yearsOfEmployment];
        NSLog(@"mikey has a BMI of %.2f, and has worked with us for %.2f years", bmi, years);
         */
        
        
        //create an array of BNREmployee objects
        NSMutableArray *employees = [[NSMutableArray alloc] init];
        
        //create a dictionary of executives
        NSMutableDictionary *executives = [[NSMutableDictionary alloc] init];
        
        for(int i = 0; i < 10; i++){
            //create an instance of BNREmployee
            BNREmployee *mikey = [[BNREmployee alloc] init];
            
            //give the instance vatiables interesting names
            mikey.weightInKilos = 90 + i;
            mikey.heightInMeters = 1.8 - i/10.0;
            mikey.employeeID = i;
            
            //put the employee in the employees array
            [employees addObject:mikey];
            
            //is this the first employee?
            if(i == 0){
                [executives setObject:mikey forKey:@"CEO"];
            }
            //is this the second employee
            if(i == 1){
                [executives setObject:mikey forKey:@"CTO"];
            }
        }
        
        NSMutableArray *allAssets = [[NSMutableArray alloc] init];
        
        //create 10 assets
        for(int i = 0; i < 10; i++){
            //create an asset
            BNRAsset *asset = [[BNRAsset alloc] init];
            
            //give it an interesting label
            NSString *currentLabel = [NSString stringWithFormat:@"Laptop %d", i];
            asset.label = currentLabel;
            asset.resaleValue = i * 17;
            
            //get a random number between 0 and 9 inclusive
            NSUInteger randomIndex = random() % [employees count];
            
            //find that employee
            BNREmployee *randomEmployee = [employees objectAtIndex:randomIndex];
            
            //assign the asset to that employee
            [randomEmployee addAsset:asset];
            
            [allAssets addObject:asset];
        }
        
        NSSortDescriptor *voa = [NSSortDescriptor
                                 sortDescriptorWithKey:@"valueOfAssets"
                                 ascending:YES];
        NSSortDescriptor *eid = [NSSortDescriptor
                                 sortDescriptorWithKey:@"employeeID"
                                 ascending:YES];
        
        [employees sortUsingDescriptors: @[voa, eid]];
        
        NSLog(@"Employees: %@", employees);
        
        NSLog(@"Giving up ownership of one employee");
        [employees removeObjectAtIndex:5];
        
        NSLog(@"allAssets: %@", allAssets);
        
        //print out the entire dictionary
        NSLog(@"executives: %@", executives);
        
        //print out the CEO's information
        NSLog(@"CEO: %@", executives[@"CEO"]);
        executives = nil;
        
        NSPredicate *predicate = [NSPredicate predicateWithFormat:
                                  @"holder.valueOfAssets > 70"];
        NSArray *toBeReclaimed = [allAssets filteredArrayUsingPredicate:predicate];
        NSLog(@"toBeReclaimed: %@", toBeReclaimed);
        toBeReclaimed = nil;
        
        NSLog(@"Giving up ownership of arrays");
        
        allAssets = nil;
        employees = nil;
    }
    
    return 0;
}
