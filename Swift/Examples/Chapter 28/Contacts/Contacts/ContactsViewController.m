//
//  ViewController.m
//  Contacts
//
//  Created by Javid Shamloo on 2/8/16.
//  Copyright © 2016 BigNerdRanch. All rights reserved.
//

#import "ContactsViewController.h"
#import "Contacts-Swift.h"

@interface ContactsViewController ()

@property (nonatomic, readonly, strong) NSMutableArray *contacts;

@end

@implementation ContactsViewController

//I DID THE SILVER AND GOLD CHALLENGES

-(id)initWithCoder:(NSCoder *)aDecoder{
    self = [super initWithCoder:aDecoder];
    
    if(self){
        
        _contacts = [NSMutableArray array];
        
    }
    
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.contacts.count;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"ContactCell" forIndexPath:indexPath];
    
    Contact *contact = self.contacts[indexPath.row];
    
    cell.textLabel.text = contact.name;
    
    return cell;
}

-(IBAction)cancelToContactsViewController:(UIStoryboardSegue *)segue{
    //no action to take if user cancels
}
-(IBAction)createNewContact:(UIStoryboardSegue *)segue{
    NewContactViewController *newContactVC = segue.sourceViewController;
    
    NSString *firstName = newContactVC.firstNameTextField.text;
    NSString *lastName = newContactVC.lastNameTextField.text;
    
    if(firstName.length != 0 || lastName.length != 0){
        NSString *contactName = [NSString stringWithFormat:@"%@ %@", firstName, lastName];
        
        Contact *newContact = [[Contact alloc] initWithContactName:contactName];
        
        [self.contacts addObject:newContact];
        [self.tableView reloadData];
    }
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    
    UINavigationController *destination = segue.destinationViewController;
    
    if ([segue.identifier isEqualToString:@"editContactSegue"]) {
        
        //get the contact we selected...
        NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];
        Contact *contact = self.contacts[indexPath.row];
        
        //...and pass it to our view controller
        ExistingContactViewController *existingVC = [destination childViewControllers][0];
        existingVC.contact = contact;
    }
}

- (IBAction)updateExistingContact:(UIStoryboardSegue *)segue {
    [self.tableView reloadData];
}

@end
