//
//  main.c
//  BeerSong
//
//  Created by Javid Shamloo on 6/17/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#include <stdio.h>

void singSongFor(int numOfBottles){
    if(numOfBottles == 0){
        printf("There are simply no more bottles of beer on the wall.\n\n");
    }
    else{
        printf("%d bottles of beer on the wall.  %d bottles of beer.\n",
               numOfBottles, numOfBottles);
        
        int oneFewer = numOfBottles - 1;
        printf("Take one down, pass it around, %d bottles of beer on the wall.\n\n", oneFewer);
        
        //recursion
        singSongFor(oneFewer);
        
        //print a message just before the function ends
        printf("Put a bottle in the recycling, %d empty bottles in the bin.\n",
               numOfBottles);
    }
}

int main(int argc, const char * argv[]) {
    singSongFor(4);
    return 0;
}
