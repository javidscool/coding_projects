//
//  ReviewsStore.swift
//  Blim-App
//
//  Created by Javid Shamloo on 7/12/16.
//  Copyright © 2016 Televisa. All rights reserved.
//

import UIKit

class ReviewStore{
    var allReviews = [Review]()
    
    func createReview(userID: Int, reviewID: Int, title: String, body: String) -> Review {
        let newReview = Review(userID: userID, reviewID: reviewID, title: title, body: body)
        
        allReviews.append(newReview)
        
        return newReview
    }
    
    let session: NSURLSession = {
        let config = NSURLSessionConfiguration.defaultSessionConfiguration()
        return NSURLSession(configuration: config)
    }()
    
    func fetchReviews(completion completion: (ReviewResult) -> Void){
        
        let url = NetworkRetrieval.postsURL()
        let request = NSURLRequest(URL: url)
        let task = session.dataTaskWithRequest(request){
            (data, response, error) -> Void in
            
            var result = self.processReviewsRequest(data: data, error: error)
            
            if case let .Success(reviews) = result {
                print("We successfully got reviews")
                result = .Success(reviews)
            }
            else{
                result = .Failure(error!)
            }
            
            completion(result)
        }
        
        task.resume()
    }
    
    func processReviewsRequest(data data: NSData?, error: NSError?) -> ReviewResult{
        guard let jsonData = data else{
            return .Failure(error!)
        }
        
        return NetworkRetrieval.reviewsFromJSONData(jsonData)
    }
}