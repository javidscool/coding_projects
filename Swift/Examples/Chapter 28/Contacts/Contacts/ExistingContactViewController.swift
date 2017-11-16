//
//  ExistingContactViewController.swift
//  Contacts
//
//  Created by Javid Shamloo on 3/15/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import UIKit

class ExistingContactViewController: UIViewController {

    @IBOutlet var firstNameTextField: UITextField!
    @IBOutlet var lastNameTextField: UITextField!
    
    var contact: Contact!{
        didSet{
            navigationItem.title = contact.name
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        var arr = contact.name.characters.split(separator: " ").map(String.init)
        
        firstNameTextField.text = arr[0]
        lastNameTextField.text = arr[1]
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        
        //clear first responder
        view.endEditing(true)
        
        //save changes to contact
        contact.name = "\(firstNameTextField.text ?? "") \(lastNameTextField.text ?? "")"
    }
}
