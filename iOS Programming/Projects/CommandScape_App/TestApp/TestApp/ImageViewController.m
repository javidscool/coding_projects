//
//  ViewController.m
//  TestApp
//
//  Created by Javid Shamloo on 10/2/16.
//  Copyright © 2016 CommandScape. All rights reserved.
//

#import "ImageViewController.h"
#import "ImageItem.h"
#import "MyCell.h"

@interface ImageViewController () <NSURLSessionDataDelegate>

@property (nonatomic, strong) NSURLSession *session;

@property (nonatomic) NSMutableArray *imageItems;

@end

NSString *const requestString = @"https://cscodechallenge.com/images";

@implementation ImageViewController

-(instancetype)initWithStyle:(UITableViewStyle)style{
    self = [super initWithStyle:style];
    
    if(self){
        NSURLSessionConfiguration *config =
            [NSURLSessionConfiguration defaultSessionConfiguration];
        
        _session = [NSURLSession sessionWithConfiguration:config
                                                 delegate:self
                                            delegateQueue:nil];
        
        [self fetchImageItems];
    }
    
    return self;
}

-(void)fetchImageItems{
    NSURL *url = [NSURL URLWithString:requestString];
    NSURLRequest *req = [NSURLRequest requestWithURL:url];
    
    NSURLSessionDataTask *dataTask =
    [self.session dataTaskWithRequest:req
                    completionHandler:
     ^(NSData *data, NSURLResponse *response, NSError *error){
         NSDictionary *jsonObject = [NSJSONSerialization JSONObjectWithData:data
                                                                    options:0
                                                                      error:nil];
         //NSLog(@"%@", jsonObject);
         
         NSArray *images = jsonObject[@"images"];
         
         for(int i = 0; i < [images count]; i++){
             [_imageItems addObject:[[ImageItem alloc]
                                initWithTitleText:images[i][@"title_text"]
                                         bodyText:images[i][@"body_text"]
                                         imageURL:images[i][@"image_url"]]];
         }
         
         dispatch_async(dispatch_get_main_queue(), ^{
             [self.tableView reloadData];
         });
     }];
    [dataTask resume];
}

-(void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition, NSURLCredential * _Nullable))completionHandler{
    
    //NSLog(@"Inside authentication challenge");
    
    // Load Certificate
    NSString *path = [[NSBundle mainBundle] pathForResource:@"client" ofType:@"p12"];
    NSData *p12data = [NSData dataWithContentsOfFile:path];
    CFDataRef inP12data = (__bridge CFDataRef)p12data;
    
    SecIdentityRef myIdentity;
    SecTrustRef myTrust;
    [self getIdentityAndTrust:inP12data identity:&myIdentity trust:&myTrust];
    
    SecCertificateRef myCertificate;
    SecIdentityCopyCertificate(myIdentity, &myCertificate);
    const void *certs[] = { myCertificate };
    CFArrayRef certsArray = CFArrayCreate(NULL, certs, 1, NULL);
    
    NSURLCredential *credential = [NSURLCredential
                                   credentialWithIdentity:myIdentity
                                   certificates:(__bridge NSArray*)certsArray
                                   persistence:NSURLCredentialPersistenceForSession];
    
    completionHandler(NSURLSessionAuthChallengeUseCredential, credential);
    
}

-(OSStatus)getIdentityAndTrust:(CFDataRef)inP12data
                          identity:(SecIdentityRef *)identity
                             trust:(SecTrustRef *)trust
{
    OSStatus securityError = errSecSuccess;
    
    CFStringRef password = CFSTR("commandscape");
    const void *keys[] = { kSecImportExportPassphrase };
    const void *values[] = { password };
    
    CFDictionaryRef options = CFDictionaryCreate(NULL, keys, values, 1, NULL, NULL);
    
    CFArrayRef items = CFArrayCreate(NULL, 0, 0, NULL);
    securityError = SecPKCS12Import(inP12data, options, &items);
    
    if (securityError == 0) {
        CFDictionaryRef myIdentityAndTrust = (CFDictionaryRef)CFArrayGetValueAtIndex(items, 0);
        
        const void *tempId = NULL;
        tempId = CFDictionaryGetValue(myIdentityAndTrust, kSecImportItemIdentity);
        *identity = (SecIdentityRef)tempId;
        
        const void *tempTrust = NULL;
        tempTrust = CFDictionaryGetValue(myIdentityAndTrust, kSecImportItemTrust);
        *trust = (SecTrustRef)tempTrust;
    }
    
    if (options) {
        CFRelease(options);
    }
    
    return securityError;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    _imageItems = [NSMutableArray array];
    
    [self.tableView registerClass:[MyCell class]
           forCellReuseIdentifier:@"UITableViewCell"];
    
    
    //fitting insets to not overlap top
    CGFloat statusBarHeight = [[UIApplication sharedApplication] statusBarFrame].size.height;
    
    UIEdgeInsets insets = UIEdgeInsetsMake(statusBarHeight, 0, 0, 0);
    self.tableView.contentInset = insets;
    self.tableView.scrollIndicatorInsets = insets;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(NSInteger)tableView:(UITableView *)tableView
numberOfRowsInSection:(NSInteger)section{
    return [self.imageItems count];
}

-(UITableViewCell *)tableView:(UITableView *)tableView
        cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    MyCell *cell =
    [tableView dequeueReusableCellWithIdentifier:@"UITableViewCell" forIndexPath:indexPath];

    cell.textLabel.text = [self.imageItems[indexPath.row] titleText];
    cell.detailTextLabel.text = [self.imageItems[indexPath.row] bodyText];
    
    cell.detailTextLabel.numberOfLines = 0;
    cell.detailTextLabel.lineBreakMode = NSLineBreakByWordWrapping;
    
    //to avoid memory leak
    __weak MyCell *weakCell = cell;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
    
        MyCell *strongCell = weakCell;
        
        UIImage *img;
        
        if(strongCell.imageView.image == nil){
            //get image
            NSURL *url = [NSURL URLWithString:[self.imageItems[indexPath.row] imageURL]];
            NSData *data = [NSData dataWithContentsOfURL:url];
            img = [[UIImage alloc] initWithData:data];
            
            [[strongCell imageView] setImage:img];
            [strongCell setNeedsLayout];
        }
        else{
            [strongCell setNeedsLayout];
        }
        
    });
    
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSString *cellText = [self.imageItems[indexPath.row] bodyText];
    UIFont *cellFont = [UIFont fontWithName:@"Helvetica" size:17.0];
    CGSize constraintSize = CGSizeMake(280.0f, MAXFLOAT);
    
    CGRect labelSize = [cellText boundingRectWithSize:constraintSize
                                             options:NSStringDrawingUsesLineFragmentOrigin| NSStringDrawingUsesFontLeading
                                          attributes:@{NSFontAttributeName:cellFont}
                                             context:nil];
    return labelSize.size.height + 20;
}

@end
