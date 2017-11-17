//
//  BirdsAndFishViewController.m
//  SendWordNowApp
//
//  Created by Javid Shamloo on 4/10/17.
//  Copyright © 2017 SendWordNow. All rights reserved.
//

#import "BirdsAndFishViewController.h"
#import "ZooAnimals.h"

@interface BirdsAndFishViewController ()

@end

@implementation BirdsAndFishViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    NSArray *keys = [[[ZooAnimals sharedZoo] allAnimals] allKeys];
    NSDictionary *animals = [[ZooAnimals sharedZoo] allAnimals];
    
    if([ZooAnimals sharedZoo].indexPath.section == 1){
        
        self.title = [NSString stringWithFormat:@"%@", keys[1]];
        
        NSArray *birds = [animals valueForKeyPath:keys[1]];
        NSArray *bird = birds[[ZooAnimals sharedZoo].indexPath.row];
        
        self.nameLabel.text = [NSString stringWithFormat:@"%@", bird[0]];
    }
    else{
        self.title = [NSString stringWithFormat:@"%@", keys[2]];
        
        NSArray *fishes = [animals valueForKeyPath:keys[2]];
        NSArray *fish = fishes[[ZooAnimals sharedZoo].indexPath.row];
        
        self.nameLabel.text = [NSString stringWithFormat:@"%@", fish[0]];
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
