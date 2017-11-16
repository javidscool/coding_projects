//
//  AppDelegate.h
//  iTahDoodle
//
//  Created by Javid Shamloo on 6/22/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <UIKit/UIKit.h>

//declare a helper function that you will use to get a path to the
//location on disk where you can save the to-do list
NSString *BNRDocPath();

@interface BNRAppDelegate : UIResponder
    <UIApplicationDelegate, UITableViewDataSource>

@property (strong, nonatomic) UIWindow *window;

@property (nonatomic) UITableView *taskTable;
@property (nonatomic) UITextField *taskField;
@property (nonatomic) UIButton *insertButton;

@property (nonatomic) NSMutableArray *tasks;

-(void)addTask:(id)sender;


@end

