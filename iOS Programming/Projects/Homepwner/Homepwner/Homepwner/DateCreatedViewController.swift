//
//  DateCreatedViewController.swift
//  Homepwner
//
//  Created by Javid Shamloo on 3/21/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import UIKit

class DateCreatedViewController : UIViewController{
    var datePicker: UIDatePicker!
    var item: Item!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.title = "Date Created"
        
        //set up our date picker
        datePicker = UIDatePicker()
        datePicker.datePickerMode = .date
        datePicker.date = item.dateCreated
        datePicker.translatesAutoresizingMaskIntoConstraints = false
        self.view.addSubview(datePicker)
        
        
        //set and enable constraints
        let centerXConstraint = datePicker.centerXAnchor.constraint(
            equalTo: view.centerXAnchor)
        let centerYConstraint = datePicker.centerYAnchor.constraint(
            equalTo: view.centerYAnchor)
        
        centerXConstraint.isActive = true
        centerYConstraint.isActive = true
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        
        //set the date we picked
        item.dateCreated = datePicker.date
    }
}
