//
//  CustomTextField.swift
//  Homepwner
//
//  Created by Javid Shamloo on 3/21/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import UIKit

class CustomTextField : UITextField {
    override func becomeFirstResponder() -> Bool {
        borderStyle = .bezel
        return super.becomeFirstResponder()
    }
    
    override func resignFirstResponder() -> Bool {
        borderStyle = .roundedRect
        return super.resignFirstResponder()
    }
}
