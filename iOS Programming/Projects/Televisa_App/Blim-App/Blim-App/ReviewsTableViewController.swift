//
//  ReviewsTableViewController.swift
//  Blim-App
//
//  Created by Javid Shamloo on 7/11/16.
//  Copyright © 2016 Televisa. All rights reserved.
//

import UIKit

class ReviewsTableViewController: UITableViewController{
    
    var reviewStore: ReviewStore!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        tableView.rowHeight = UITableViewAutomaticDimension
        tableView.estimatedRowHeight = 90
        
        reviewStore.fetchReviews(){ (reviewsResult) -> Void in
            
            if case let .Success(reviews) = reviewsResult {
                print("We successfully loaded in reviews")
                NSOperationQueue.mainQueue().addOperationWithBlock(){
                    self.reviewStore.allReviews = reviews
                    self.tableView.reloadData()
                }
            }
            else{
                print("We have an error loading in users")
            }
        }
    }
    
    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
        
        self.parentViewController!.tabBarItem.selectedImage! =
            TabGraphics.singletonInstance.allGraphics[TabGraphics.TabTypes.Highlighted.rawValue]
    }
    
    override func viewWillDisappear(animated: Bool) {
        super.viewWillDisappear(animated)
        
        self.parentViewController!.tabBarItem.image! =
            TabGraphics.singletonInstance.allGraphics[TabGraphics.TabTypes.Deselected.rawValue]
        
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    
    //table view methods
    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return reviewStore.allReviews.count
    }
    
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        
        //create an instance of a cell, with default appearance
        let cell = UITableViewCell(style: .Subtitle, reuseIdentifier: "UITableViewCell")
        
        //set cell properties
        let review = reviewStore.allReviews[indexPath.row]
        
        cell.textLabel?.text = review.title
        cell.textLabel?.numberOfLines = 0
        cell.textLabel?.lineBreakMode = .ByWordWrapping
        
        cell.detailTextLabel?.text = review.body
        
        return cell
    }
    
    override func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        
        self.performSegueWithIdentifier("ShowReview", sender: nil)
    }
 
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        
        if segue.identifier == "ShowReview"{
            if let row = tableView.indexPathForSelectedRow?.row{
                
                //get the review associated with the tap and pass it along
                let review = reviewStore.allReviews[row]
                let reviewDetailController = segue.destinationViewController as! ReviewDetailController
                reviewDetailController.review = review
            }
        }
    }
 
}
