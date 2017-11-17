//
//  UsersStore.swift
//  Blim-App
//
//  Created by Javid Shamloo on 7/12/16.
//  Copyright © 2016 Televisa. All rights reserved.
//

import UIKit

class UserStore{
    var allUsers = [User]()
    
    func createUser(userID: Int, name: String, email: String) -> User {
        let newUser = User(userID: userID, name: name, email: email)
        
        allUsers.append(newUser)
        
        return newUser
    }
    
    let session: NSURLSession = {
        let config = NSURLSessionConfiguration.defaultSessionConfiguration()
        return NSURLSession(configuration: config)
    }()
    
    func fetchUsers(completion completion: (UserResult) -> Void){
        
        let url = NetworkRetrieval.usersURL()
        let request = NSURLRequest(URL: url)
        let task = session.dataTaskWithRequest(request){
            (data, response, error) -> Void in
            
            var result = self.processUsersRequest(data: data, error: error)
            
            if case let .Success(users) = result {
                print("We successfully got users")
                result = .Success(users)
            }
            else{
                result = .Failure(error!)
            }
            
            completion(result)
        }
        
        task.resume()
    }
    
    func fetchUserReviews(userID: Int, completion: (ReviewResult) -> Void){
        
        let url = NetworkRetrieval.userPostsURL(userID)
        let request = NSURLRequest(URL: url)
        let task = session.dataTaskWithRequest(request){
            (data, response, error) -> Void in
            
            var result = self.processUserReviewsRequest(data: data, error: error)
            
            if case let .Success(reviews) = result {
                print("We successfully got user reviews")
                result = .Success(reviews)
            }
            else{
                result = .Failure(error!)
            }
            
            completion(result)
        }
        
        task.resume()
    }
    
    func processUsersRequest(data data: NSData?, error: NSError?) -> UserResult{
        guard let jsonData = data else{
            return .Failure(error!)
        }
        
        return NetworkRetrieval.usersFromJSONData(jsonData)
    }
    
    func processUserReviewsRequest(data data: NSData?, error: NSError?) -> ReviewResult{
        guard let jsonData = data else{
            return .Failure(error!)
        }
        
        return NetworkRetrieval.reviewsFromJSONData(jsonData)
    }
}
