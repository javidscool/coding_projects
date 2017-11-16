//
//  ZooTableViewController.m
//  SendWordNowApp
//
//  Created by Javid Shamloo on 4/10/17.
//  Copyright © 2017 SendWordNow. All rights reserved.
//

#import "ZooTableViewController.h"
#import "ZooAnimals.h"
#import "MammalsViewController.h"
#import "BirdsAndFishViewController.h"

@interface ZooTableViewController ()

@end

@implementation ZooTableViewController

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    self.title = @"Animals";
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 3;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    switch(section){
        case 0:
        {
            return [[[ZooAnimals sharedZoo] allAnimals][@"Mammals"] count];
            break;
        }
        case 1:
        {
            return [[[ZooAnimals sharedZoo] allAnimals][@"Birds"] count];
            break;
        }
        default:
        {
            return [[[ZooAnimals sharedZoo] allAnimals][@"Fish"] count];
            break;
        }
    }
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    NSArray *keys = [[[ZooAnimals sharedZoo] allAnimals] allKeys];
    
    switch(section){
        case 0:
        {
            return keys[0];
            break;
        }
        case 1:
        {
            return keys[1];
            break;
        }
        default:
        {
            return keys[2];
            break;
        }
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"UITableViewCell"];
    
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier: @"UITableViewCell"];
    }
    
    NSArray *keys = [[[ZooAnimals sharedZoo] allAnimals] allKeys];
    NSDictionary *animals = [[ZooAnimals sharedZoo] allAnimals];
    
    switch(indexPath.section){
        case 0:
        {
            NSArray *mammals = [animals valueForKeyPath:keys[0]];
            NSArray *mammal = mammals[indexPath.row];
            cell.textLabel.text = keys[0];
            cell.detailTextLabel.text = [NSString stringWithFormat:@"%@", mammal[1]];
            break;
        }
        case 1:
        {
            NSArray *birds = [animals valueForKeyPath:keys[1]];
            NSArray *bird = birds[indexPath.row];
            cell.textLabel.text = keys[1];
            cell.detailTextLabel.text = [NSString stringWithFormat:@"%@", bird[1]];
            break;
        }
        default:
        {
            NSArray *fishes = [animals valueForKeyPath:keys[2]];
            NSArray *fish = fishes[indexPath.row];
            cell.textLabel.text = keys[2];
            cell.detailTextLabel.text = [NSString stringWithFormat:@"%@", fish[1]];
            break;
        }
    }
    
    return cell;
}


-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"main"
                                                         bundle:[NSBundle mainBundle]];
    
    [ZooAnimals sharedZoo].indexPath = indexPath;
    
    if(indexPath.section == 0){
        MammalsViewController *mammalController =
        [storyboard instantiateViewControllerWithIdentifier:@"mammalVC"];
        
        [self.navigationController pushViewController:mammalController animated:YES];
    }
    else{
        
        UIViewController *birdOrFishController =
        [storyboard instantiateViewControllerWithIdentifier:@"birdOrFishVC"];
        
        [self.navigationController pushViewController:birdOrFishController animated:YES];
    }
}


// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    
     if([segue.identifier isEqualToString:@"showMammal"]){
         UINavigationController *nc = (UINavigationController *)segue.destinationViewController;
         
         MammalsViewController *mvc = (MammalsViewController *)[nc topViewController];
         
         mvc.nameLabel.text = @"mammalHolder";
     }
     else  if([segue.identifier isEqualToString:@"showBirdOrFish"]){
         UINavigationController *nc = (UINavigationController *)segue.destinationViewController;
         
         BirdsAndFishViewController *bfvc =
            (BirdsAndFishViewController *)[nc topViewController];
         
         bfvc.nameLabel.text = @"birdOrFishHolder";
     }
}


@end
