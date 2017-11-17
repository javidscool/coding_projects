//
//  Document.m
//  TahDoodle
//
//  Created by Javid Shamloo on 6/23/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "Document.h"

@interface Document ()

@end

@implementation Document

#pragma mark - NSDocument Overrides

- (instancetype)init {
    self = [super init];
    if (self) {
        // Add your subclass-specific initialization here.
    }
    return self;
}

- (void)windowControllerDidLoadNib:(NSWindowController *)aController {
    [super windowControllerDidLoadNib:aController];
    // Add any code here that needs to be executed once the windowController has loaded the document's window.
}

+ (BOOL)autosavesInPlace {
    return YES;
}

- (NSString *)windowNibName {
    // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"Document";
}

#pragma mark - Actions
-(void)addTask:(id)sender
{
    //if there is no array yet, create one
    if(!self.tasks){
        self.tasks = [NSMutableArray array];
    }
    
    [self.tasks addObject:@"New Item"];
    
    //-reloadData tells the table view to refresh and ask its dataSource
    //(which happens to be this BNRDocument object in this case)
    //for new data to display
    [self.taskTable reloadData];
    
    //-updateChangeCount: tells the application whether or not the document
    //has unsaved changes, NSChangeDone flags the document as unsaved
    [self updateChangeCount:NSChangeDone];
}

-(IBAction)deleteTask:(id)sender
{
    if ([self.taskTable selectedRow] != -1) {
        
        [self.tasks removeObjectAtIndex:[self.taskTable selectedRow]];
        
        [self.taskTable reloadData];
        
        [self updateChangeCount:NSChangeDone];
    }
}

#pragma mark Data Source Methods
-(NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
    //this table view displays the tasks array,
    //so the number of entries in the table view will be the same
    //as the number of objects in the array
    return [self.tasks count];
}

-(id)tableView:(NSTableView *)tableView
    objectValueForTableColumn:(NSTableColumn *)tableColumn
    row:(NSInteger)row
{
    //return the item from tasks that corresponds to the cell
    //that the table view wants to display
    return [self.tasks objectAtIndex:row];
}

-(void)tableView:(NSTableView *)tableView
        setObjectValue:(id)object
        forTableColumn:(NSTableColumn *)tableColumn
        row:(NSInteger)row
{
    //when the user changes a task on the table view,
    //update the tasks array
    [self.tasks replaceObjectAtIndex:row withObject:object];
    
    //and then flag the document as having unsaved changes
    [self updateChangeCount:NSChangeDone];
}

-(NSData *)dataOfType:(NSString *)typeName
                error:(NSError *__autoreleasing *)outError
{
    //this method is called when our document is being saved
    //you are expected to hand the caller an NSData object wrapping
    //our data so that it can be written to disk
    
    //if there is no array, write out an empty array
    if(!self.tasks){
        self.tasks = [NSMutableArray array];
    }
    
    //pack the tasks array into an NSData object
    NSData *data = [NSPropertyListSerialization
                    dataWithPropertyList:self.tasks
                    format:NSPropertyListXMLFormat_v1_0
                    options:0
                    error:outError];
    
    //return the newly-packed NSData object
    return data;
    
}

-(BOOL)readFromData:(NSData *)data
        ofType:(NSString *)typeName
        error:(NSError *__autoreleasing *)outError
{
    //this method is called when a document is being loaded
    //you are handed an NSData object and expected to pull our data
    //out of it
    
    //extract the tasks
    self.tasks = [NSPropertyListSerialization
                  propertyListWithData:data
                  options:NSPropertyListMutableContainers
                  format:NULL
                  error:outError];
    
    //return success or failure depending on success of the above call
    return (self.tasks != nil);
}

@end
