//
//  Document.h
//  TahDoodle
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface Document : NSDocument
    <NSTableViewDataSource>

@property (nonatomic) NSMutableArray *tasks;

@property (nonatomic) IBOutlet NSTableView *taskTable;

-(IBAction)addTask:(id)sender;

-(IBAction)deleteTask:(id)sender;

@end

