//
//  MammalsViewController.m
//  SendWordNowApp
//
//  Created by Javid Shamloo on 4/10/17.
//  Copyright © 2017 SendWordNow. All rights reserved.
//

#import "MammalsViewController.h"
#import "ZooAnimals.h"

@interface MammalsViewController ()

@end

@implementation MammalsViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    NSArray *keys = [[[ZooAnimals sharedZoo] allAnimals] allKeys];
    
    self.title = [NSString stringWithFormat:@"%@", keys[0]];
    
    NSDictionary *animals = [[ZooAnimals sharedZoo] allAnimals];
    NSArray *mammals = [animals valueForKeyPath:keys[0]];
    NSArray *mammal = mammals[[ZooAnimals sharedZoo].indexPath.row];
    
    self.nameLabel.text = [NSString stringWithFormat:@"%@", mammal[0]];
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
