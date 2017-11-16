//
//  main.c
//  ClassCertificates
//
//  Created by Javid Shamloo on 6/17/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#include <stdio.h>

void congradulateStudent(char *student, char *course, int numDays){
    printf("%s has done as much %s Programming as I could fit into %d days.\n", student, course, numDays);
}

int main(int argc, const char * argv[]) {
    congradulateStudent("Kate", "Cocoa", 5);
    congradulateStudent("Bo", "Objective-C", 2);
    congradulateStudent("Mike", "Python", 5);
    congradulateStudent("Liz", "iOS", 5);
    
    return 0;
}
