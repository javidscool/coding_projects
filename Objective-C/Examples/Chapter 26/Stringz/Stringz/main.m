//
//  main.m
//  Stringz
//
//  Created by Javid Shamloo on 6/20/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        //WRITING TO A FILE
        /*
        NSMutableString *str = [[NSMutableString alloc] init];
        
        for(int i = 0; i < 10; i++){
            [str appendString:@"Aaron is cool!\n"];
        }
        
        //declare a pointer to an NSError object, but don't instantiate it
        //the NSError will only be created if necessary
        NSError *error;
        
        //get the error is there is one
        BOOL success = [str writeToFile:@"/tmp/cool.txt"
                            atomically:YES
                            encoding:NSUTF8StringEncoding
                            error:&error];
        
        //test the returned BOOL, and query the NSError is the write failed
        if(success){
            NSLog(@"done writing /tmp/cool.txt");
        }
        else{
            NSLog(@"writing /tmp/cool.txt failed: %@", [error localizedDescription]);
        }
         */
        
        
        //READING IN A FILE
        NSError *error;
        NSString *str = [[NSString alloc] initWithContentsOfFile:@"/etc/resolv.conf"
                                                        encoding:NSASCIIStringEncoding
                                                           error:&error];
        
        if(!str){
            NSLog(@"read failed: %@", [error localizedDescription]);
        }
        else{
            NSLog(@"resolv.conf looks like this: %@", str);
        }
        
    }
    return 0;
}
