//
//  NetworkRetrieval.swift
//  Blim-App
//
//  Created by Javid Shamloo on 7/12/16.
//  Copyright © 2016 Televisa. All rights reserved.
//

import UIKit

enum Method: String{
    case Users = "users"
    case Posts = "posts"
}

enum ReviewResult{
    case Success([Review])
    case Failure(ErrorType)
}

enum UserResult{
    case Success([User])
    case Failure(ErrorType)
}

enum JsonError: ErrorType{
    case InvalidJSONData
}

struct NetworkRetrieval{
    
    private static let baseURLString = "https://jsonplaceholder.typicode.com/"
    
    
    //creating our URL
    private static func constructedURL(method method: Method, parameters: [String]?) -> NSURL{
        
        var stringURL: String = baseURLString
        
        stringURL += method.rawValue
        
        if let additionalParameters = parameters{
            for param in additionalParameters{
                stringURL += "/\(param)"
            }
        }
        
        let components = NSURLComponents(string: stringURL)!
        
        return components.URL!
    }
    
    static func usersURL() -> NSURL{
        return constructedURL(method: .Users, parameters: nil)
    }
    
    static func postsURL() -> NSURL{
        return constructedURL(method: .Posts, parameters: nil)
    }
    
    static func userPostsURL(userID: Int) -> NSURL{
        return constructedURL(method: .Users, parameters: [String(userID), "posts"])
    }
    
    static func usersFromJSONData(data: NSData) -> UserResult{
        do{
            let jsonObject: AnyObject = try NSJSONSerialization.JSONObjectWithData(data, options: [])
            
            guard let
                userArray = jsonObject as? [[String:AnyObject]] else{
                    
                    //the JSON structure doesn't match our expectations
                    return .Failure(JsonError.InvalidJSONData)
            }
            
            var finalUsers = [User]()
            
            for userJSON in userArray{
                if let user = userFromJSONObject(userJSON){
                    finalUsers.append(user)
                }
            }
            
            if finalUsers.count == 0{
                //we weren't able to parse any of the users
                //maybe the JSON format for users has changed
                return .Failure(JsonError.InvalidJSONData)
            }
            
            return .Success(finalUsers)
        }
        catch let error{
            return .Failure(error)
        }
    }
    
    private static func userFromJSONObject(json: [String:AnyObject]) -> User?{
        
        guard let userID = json["id"] as? Int,
            name = json["name"] as? String,
            email = json["email"] as? String else{
                //don't have enough info to construct a user
                return nil
        }
        
        let user: User = User(userID: userID, name: name, email: email)
        
        return user
    }
    
    
    static func reviewsFromJSONData(data: NSData) -> ReviewResult{
        do{
            let jsonObject: AnyObject = try NSJSONSerialization.JSONObjectWithData(data, options: [])
            
            guard let
                reviewArray = jsonObject as? [[String:AnyObject]] else{
                    
                    //the JSON structure doesn't match our expectations
                    return .Failure(JsonError.InvalidJSONData)
            }
            
            var finalReviews = [Review]()
            
            for reviewJSON in reviewArray{
                if let review = reviewFromJSONObject(reviewJSON){
                    finalReviews.append(review)
                }
            }
            
            if finalReviews.count == 0{
                //we weren't able to parse any of the users
                //maybe the JSON format for users has changed
                return .Failure(JsonError.InvalidJSONData)
            }
            
            return .Success(finalReviews)
        }
        catch let error{
            return .Failure(error)
        }
    }
    
    private static func reviewFromJSONObject(json: [String:AnyObject]) -> Review?{
        
        guard let userID = json["userId"] as? Int,
            reviewID = json["id"] as? Int,
            title = json["title"] as? String,
            body = json["body"] as? String else{
                //don't have enough info to construct a review
                return nil
        }
        
        let review: Review = Review(userID: userID, reviewID: reviewID, title: title, body: body)
        
        return review
    }

}
