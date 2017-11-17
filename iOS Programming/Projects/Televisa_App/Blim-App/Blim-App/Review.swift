//
//  Reviews.swift
//  Blim-App
//
//  Created by Javid Shamloo on 7/12/16.
//  Copyright © 2016 Televisa. All rights reserved.
//

import UIKit

class Review: NSObject{
    
    var userID: Int
    var reviewID: Int
    var title: String
    var body: String
    
    init(userID: Int, reviewID: Int, title: String, body: String){
        self.userID = userID
        self.reviewID = reviewID
        self.title = title
        self.body = body
        
        super.init()
    }
    
}