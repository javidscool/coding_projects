//
//  main.m
//  FindCommonNames
//
//  Created by Javid Shamloo on 6/19/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        //read in a file as a huge string
        NSString *nameString =
            [NSString stringWithContentsOfFile:@"/usr/share/dict/propernames"
                                      encoding:NSUTF8StringEncoding
                                         error:NULL];
        
        //break it into an array of strings
        NSArray *names = [nameString componentsSeparatedByString:@"\n"];
        
        //read in a file as a huge string
        NSString *wordString =
        [NSString stringWithContentsOfFile:@"/usr/share/dict/words"
                                  encoding:NSUTF8StringEncoding
                                     error:NULL];
        
        //break it into an array of strings
        NSArray *words = [wordString componentsSeparatedByString:@"\n"];
        
        
        //go thru the words array
        for(NSString *w in words){
            
            //for every word in the words array, we go thru the names array
            for(NSString *n in names){
                //find the name that's also in words
                NSRange r = [w rangeOfString:n options:NSLiteralSearch];
                
                //was it found?
                if(r.location != NSNotFound){
                    NSLog(@"%@", n);
                }
            }
        }
        
        
        /*
        //go thru the array one string at a time
        for(NSString *n in names){
            //look for the string "aa" in a case-insensitive manner
            NSRange r = [n rangeOfString:@"AA"
                                 options:NSCaseInsensitiveSearch];
            
            //was it found?
            if(r.location != NSNotFound){
                NSLog(@"%@", n);
            }
        }
         */
        
    }
    return 0;
}
