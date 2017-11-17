//
//  ImageItem.m
//  TestApp
//
//  Created by Javid Shamloo on 10/2/16.
//  Copyright © 2016 CommandScape. All rights reserved.
//

#import "ImageItem.h"

@implementation ImageItem

-(instancetype)initWithTitleText:(NSString *)titletxt
                        bodyText:(NSString *)bodyTxt
                        imageURL:(NSString *)imgURL;
{
    
    //call NSObject's init method
    if(self = [super init]){
        _titleText = titletxt;
        _bodyText = bodyTxt;
        _imageURL = imgURL;
    }
    
    return self;
}

@end
