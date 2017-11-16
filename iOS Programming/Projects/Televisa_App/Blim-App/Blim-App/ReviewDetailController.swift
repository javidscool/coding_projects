//
//  ReviewDetailController.swift
//  Blim-App
//
//  Created by Javid Shamloo on 7/12/16.
//  Copyright © 2016 Televisa. All rights reserved.
//

import UIKit

class ReviewDetailController: UIViewController{
    @IBOutlet var reviewTitle: UILabel!
    @IBOutlet var reviewBody: UITextView!
    
    var review: Review!{
        didSet{
            navigationItem.title = "Reviews"
        }
    }
    
    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
        
        reviewTitle.text = review.title
        reviewTitle.sizeToFit()
        
        reviewBody.text = review.body
    }
    
}
