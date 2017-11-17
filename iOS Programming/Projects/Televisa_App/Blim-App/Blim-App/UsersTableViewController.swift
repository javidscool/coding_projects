//
//  UsersTableView.swift
//  Blim-App
//
//  Created by Javid Shamloo on 7/11/16.
//  Copyright © 2016 Televisa. All rights reserved.
//

import UIKit

class UsersTableViewController: UITableViewController{
    
    var userStore: UserStore!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        userStore.fetchUsers(){ (usersResult) -> Void in
            
            if case let .Success(users) = usersResult {
                print("We successfully loaded in users")
                NSOperationQueue.mainQueue().addOperationWithBlock(){
                    self.userStore.allUsers = users
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
        return userStore.allUsers.count
    }
    
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        
        //create an instance of a cell, with default appearance
        let cell = UITableViewCell(style: .Subtitle, reuseIdentifier: "UITableViewCell")
        
        //set cell properties
        let user = userStore.allUsers[indexPath.row]
        
        cell.textLabel?.text = user.name
        cell.detailTextLabel?.text = user.email
        
        return cell
    }
    
    override func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        
        self.performSegueWithIdentifier("ShowUserReviews", sender: nil)
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        
        if segue.identifier == "ShowUserReviews"{
            if let row = tableView.indexPathForSelectedRow?.row{
                
                //get the review associated with the tap and pass it along
                let user = userStore.allUsers[row]
                let userReviewDetailController = segue.destinationViewController as! UserReviewsTableViewController
                userReviewDetailController.user = user
                userReviewDetailController.userStore = userStore
            }
        }
    }
}
