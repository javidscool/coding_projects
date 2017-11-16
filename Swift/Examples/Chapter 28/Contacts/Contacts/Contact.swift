//
//  Contacts.swift
//  Contacts
//
//  Created by Javid Shamloo on 2/8/16.
//  Copyright © 2016 BigNerdRanch. All rights reserved.
//

import Foundation

class Contact: NSObject{
    var name: String
    init(contactName: String){
        name = contactName
        super.init()
    }
}
