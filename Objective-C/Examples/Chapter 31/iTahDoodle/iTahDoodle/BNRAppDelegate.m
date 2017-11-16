//
//  AppDelegate.m
//  iTahDoodle
//
//  Created by Javid Shamloo on 6/22/15.
//  Copyright (c) 2015 Javid Shamloo. All rights reserved.
//

#import "BNRAppDelegate.h"

//helper function to fetch the path to our to-do data stored on disk
NSString *BNRDocPath(){
    NSArray *pathList = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                                            NSUserDomainMask,
                                                            YES);
    
    return [pathList[0] stringByAppendingPathComponent:@"data.td"];
}

@interface BNRAppDelegate ()

@end

@implementation BNRAppDelegate

#pragma mark - Application delegate callbacks

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    //load an existing dataset or create a new one
    NSArray *plist = [NSArray arrayWithContentsOfFile:BNRDocPath()];
    if(plist){
        //we have a dataset; copy it into tasks
        self.tasks = [plist mutableCopy];
    }
    else{
        //there is no dataset; create an empty array
        self.tasks = [NSMutableArray array];
    }
    
    //create and configure the UIWindow instance
    //a CGRect is a struct with an origin (x, y) and a size (width, height)
    CGRect winFrame = [[UIScreen mainScreen] bounds];
    UIWindow *theWindow = [[UIWindow alloc] initWithFrame:winFrame];
    self.window = theWindow;
    
    //define the frame rectangles of the three UI elements
    //CGRectMake() creates a CGRect from (x, y, width, height)
    CGRect tableFrame = CGRectMake(0, 80, winFrame.size.width, winFrame.size.height - 100);
    CGRect fieldFrame = CGRectMake(20, 40, 200, 31);
    CGRect buttonFrame = CGRectMake(228, 40, 72, 31);
    
    //create and configure the UITableView instance
    self.taskTable = [[UITableView alloc] initWithFrame:tableFrame
                                                  style:UITableViewStylePlain];
    self.taskTable.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    //make the BNRAppDelegate the table view's dataSource
    self.taskTable.dataSource = self;
    
    //tell the table view which class to instantiate whenever it needs to create a new cell
    [self.taskTable registerClass:[UITableViewCell class] forCellReuseIdentifier:@"Cell"];
    
    //create and configure the UITextField instance where new tasks will be entered
    self.taskField = [[UITextField alloc] initWithFrame:fieldFrame];
    self.taskField.borderStyle = UITextBorderStyleRoundedRect;
    self.taskField.placeholder = @"Type a task, tap Insert";
    
    //create and configure the UIButton instance
    self.insertButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    self.insertButton.frame = buttonFrame;
    
    //give the button a title
    [self.insertButton setTitle:@"Insert" forState:UIControlStateNormal];
    
    //set the target and action for the insert button
    [self.insertButton addTarget:self
                    action:@selector(addTask:)
                    forControlEvents:UIControlEventTouchUpInside];
    
    //add our three UI elements to the window
    [self.window addSubview:self.taskTable];
    [self.window addSubview:self.taskField];
    [self.window addSubview:self.insertButton];
    
    //finialize the window and put it on the screen
    self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    //save our tasks array to disk
    [self.tasks writeToFile:BNRDocPath() atomically:YES];
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

#pragma mark - Application delegate callbacks

-(void)addTask:(id)sender
{
    //get the task
    NSString *text = [self.taskField text];
    
    //quit here if taskfield is empty
    if([text length] == 0){
        //clear out the text field
        [self.taskField setText:@""];
        
        //dismiss the keyboard
        [self.taskField resignFirstResponder];
        
        return;
    }
    
    //add it to theworking array
    [self.tasks addObject:text];
    
    //refresh the table so that the new item shows up
    [self.taskTable reloadData];
    
    //clear out the text field
    [self.taskField setText:@""];
    
    //dismiss the keyboard
    [self.taskField resignFirstResponder];
}


-(NSInteger)tableView:(UITableView *)tableView
numberOfRowsInSection:(NSInteger)section
{
    //because this table view only has one section,
    //the number of rows in it is equal to the number of items in the tasks array
    return [self.tasks count];
}

-(UITableViewCell *)tableView:(UITableView *)tableView
        cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    //to improve performance, this method first checks
    //for an existing cell object that we can reuse
    UITableViewCell *c = [self.taskTable dequeueReusableCellWithIdentifier:@"Cell"];
    
    //if there isn't one, then a new cell is created
    if(!c){
        c = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
                                   reuseIdentifier:@"Cell"];
    }
    
    //then we (re)configure the cell based on the model object, in this case the tasks array
    NSString *item = [self.tasks objectAtIndex:indexPath.row];
    c.textLabel.text = item;
    
    //...and hand the properly configured cell back to the table view
    return c;
}

@end
