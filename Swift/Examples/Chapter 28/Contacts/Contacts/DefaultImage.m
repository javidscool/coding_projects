//
//  DefaultImage.m
//  Contacts
//
//  Created by Javid Shamloo on 2/8/16.
//  Copyright © 2016 BigNerdRanch. All rights reserved.
//

#import "DefaultImage.h"

@implementation DefaultImage

+(UIImage *)generateDefaultImageOfSize:(CGSize)size{
    //make frame
    CGRect frame = CGRectMake(0, 0, size.width, size.height);
    
    //get image context
    UIGraphicsBeginImageContext(size);
    
    //get context reference
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    //draw white background to avoid default black
    CGColorRef white = [[UIColor whiteColor] CGColor];
    CGContextSetFillColorWithColor(context, white);
    CGContextFillRect(context, frame);
    
    //make yellow circle
    CGColorRef yellow = [[UIColor yellowColor] CGColor];
    CGContextSetFillColorWithColor(context, yellow);
    CGContextFillEllipseInRect(context, frame);
    
    //center of circle
    CGFloat x = frame.origin.x + size.width / 2;
    CGFloat y = frame.origin.y + size.height / 2;
    CGPoint center = CGPointMake(x, y);
    
    //draw eyes
    CGColorRef black = [[UIColor blackColor] CGColor];
    CGRect leftEyeRect = CGRectMake(center.x - 50, center.y - 50, 20, 20);
    CGRect rightEyeRect = CGRectMake(center.x + 30, center.y - 50, 20, 20);
    CGContextSetFillColorWithColor(context, black);
    CGContextFillEllipseInRect(context, leftEyeRect);
    CGContextFillEllipseInRect(context, rightEyeRect);
    
    //draw smile
    CGContextSetLineWidth(context, 5.0);
    CGContextBeginPath(context);
    CGContextMoveToPoint(context, center.x - 50, center.y + 35);
    CGContextAddCurveToPoint(context, center.x - 25, center.y + 50, center.x + 25, center.y + 50, center.x + 50, center.y + 35);
    CGContextStrokePath(context);
    
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return image;
}

@end
