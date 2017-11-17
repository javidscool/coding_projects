//
//  main.c
//  Turkey
//
//  Created by Javid Shamloo on 6/17/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    //declare the variable called "weight" of type float
    float weight;
    
    //store a number in that variable
    weight = 14.2;
    
    //log it to the user
    printf("The turkey weighs %f.\n", weight);
    
    //declare another variable of type float
    float cookingTime;
    
    //calculate the cooking time and store it in the variable
    //in this case, '*' means 'multiplied by'
    cookingTime = 15.0 + 15.0 * weight;
    
    //log that to the user
    printf("Cook it for %f minutes.\n", cookingTime);
    
    //end this function and indicate success
    return 0;
}
