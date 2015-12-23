//
//  ViewController.m
//  SDKPlainSampleApp
//
//  Created by Vishwesh Joshi on 15/12/15.
//  Copyright © 2015 Robosoft. All rights reserved.
//

#import "ViewController.h"
#import <TeamchatSDK/TeamchatSDK.h>

@interface ViewController ()

@end

@implementation ViewController
{
    UIViewController *loginController;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // This is just a Simple Demo.
    // Do any other setup accordingly as per documentation.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    // This is just a DEMO
    // Replace the snipped accordingly
    if (!loginController)
    {
        loginController = [Teamchat teamchatLoginViewController:^(BOOL success, NSError *error, NSString *errMsg) {
            if (error)
            {
                [loginController dismissViewControllerAnimated:YES completion:nil];
            }
        }];
        [self presentViewController:loginController animated:YES completion:nil];
    }
}


@end
