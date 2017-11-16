//
//  ImageItem.h
//  TestApp
//
//  Created by Javid Shamloo on 10/2/16.
//  Copyright © 2016 CommandScape. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface ImageItem : NSObject

@property (nonatomic, strong) NSString * titleText;
@property (nonatomic, strong) NSString * bodyText;
@property (nonatomic, strong) NSString * imageURL;
@property (nonatomic) UIImage *thumbnail;

-(instancetype)initWithTitleText:(NSString *)titletxt
                        bodyText:(NSString *)bodtTxt
                        imageURL:(NSString *)imgURL;

@end
