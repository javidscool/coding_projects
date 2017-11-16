//
//  Users.swift
//  Blim-App
//
//  Created by Javid Shamloo on 7/12/16.
//  Copyright © 2016 Televisa. All rights reserved.
//

import UIKit

class User: NSObject{
    
    var userID: Int
    var name: String
    var email: String
    var reviews: [Review]?
    
    init(userID: Int, name: String, email: String){
        self.userID = userID
        self.name = name
        self.email = email
        
        super.init()
    }
    
}