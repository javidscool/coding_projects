//
//  main.c
//  Degrees
//
//  Created by Javid Shamloo on 6/17/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#include <stdio.h>

float fahrenheitFromCelsius(float cel){
    float fahr = cel * 1.8 + 32;
    printf("%f Celsius is %f Fahrenheit\n", cel, fahr);
    return fahr;
}

int main(int argc, const char * argv[]) {
    float freezeInC = 0;
    float freezeInF = fahrenheitFromCelsius(freezeInC);
    printf("Water freezes at %f degrees Fahrenheit.\n", freezeInF);
    return 0;
}
