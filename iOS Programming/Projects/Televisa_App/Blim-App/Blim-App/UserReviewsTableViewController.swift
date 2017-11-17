//
//  UserReviewsTableViewController.swift
//  Blim-App
//
//  Created by Javid Shamloo on 7/12/16.
//  Copyright © 2016 Televisa. All rights reserved.
//

import UIKit

class UserReviewsTableViewController: UITableViewController{
    
    var user: User!{
        didSet{
            navigationItem.title = "Reviews by \(user.name)"
        }
    }
    var userStore: UserStore!
    var userReviews = [Review]()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        tableView.rowHeight = UITableViewAutomaticDimension
        tableView.estimatedRowHeight = 90
        
        self.userStore.fetchUserReviews(self.user.userID){ (reviewsResult) -> Void in
            
            if case let .Success(reviews) = reviewsResult {
                print("We successfully loaded in the reviews of the user")
                NSOperationQueue.mainQueue().addOperationWithBlock(){
                    self.userReviews = reviews
                    self.tableView.reloadData()
                }
            }
            else{
                print("We have an error loading in the reviews of the user")
            }
        }
    }
    
    //table view methods
    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return userReviews.count
    }
    
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        
        //create an instance of a cell, with default appearance
        let cell = UITableViewCell(style: .Subtitle, reuseIdentifier: "UITableViewCellReview")
        
        //set cell properties
        let review = userReviews[indexPath.row]
        
        cell.textLabel?.text = review.title
        cell.textLabel?.numberOfLines = 0
        cell.textLabel?.lineBreakMode = .ByWordWrapping
        
        cell.detailTextLabel?.text = review.body
        
        return cell
    }
    
    override func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        
        self.performSegueWithIdentifier("ShowUserReview", sender: nil)
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        
        if segue.identifier == "ShowUserReview"{
            if let row = tableView.indexPathForSelectedRow?.row{
                
                //get the review associated with the tap and pass it along
                let review = userReviews[row]
                let reviewDetailController = segue.destinationViewController as! ReviewDetailController
                reviewDetailController.review = review
            }
        }
    }
}
