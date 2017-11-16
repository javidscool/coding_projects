//
//  DetailViewController.swift
//  Homepwner
//
//  Created by Javid Shamloo on 2/27/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import UIKit

//I did the bronze, silver, and gold challenges in chapter 10
//I did the bronze, silver, and gold challenges in chapter 11
//I did the bronze challenge from chapter 12
//I did the bronze, silver, and gold challenges for chapter 14
//I did the bronze, silver, and gold challenges for chapter 15
//I did the bronze challenge for chapter 16
//I did the bronze challenge for chapter 17

class DetailViewController : UIViewController, UITextFieldDelegate, UINavigationControllerDelegate, UIImagePickerControllerDelegate {
    @IBOutlet var imageView: UIImageView!
    @IBOutlet var nameField: UITextField!
    
    @IBOutlet var serialNumberField: UITextField!
    @IBOutlet var valueField: UITextField!
    @IBOutlet var dateLabel: UILabel!
    
    @IBAction func takePicture(_ sender: UIBarButtonItem) {
        let imagePicker = UIImagePickerController()
        
        //if the device has a camera, take a picture; otherwise,
        //just pick from photo library
        if UIImagePickerController.isSourceTypeAvailable(.camera){
            imagePicker.sourceType = .camera
            
            //to set the proper constraints, we must first set the orientation to portrait
            let value = UIInterfaceOrientation.portrait.rawValue
            UIDevice.current.setValue(value, forKey: "orientation")
            self.view.layoutIfNeeded()
            
            //set the overlay now
            //we can only do this if we use the camera
            let overlayView = UIView(frame: imagePicker.cameraOverlayView!.bounds)
            
            let crossHairLabel = UILabel()
            
            crossHairLabel.text = "+"
            crossHairLabel.font = UIFont.systemFont(ofSize: 50)
            crossHairLabel.translatesAutoresizingMaskIntoConstraints = false
            overlayView.addSubview(crossHairLabel)
            
            
            //set and enable overlay view constraints
            //this centers the overlay view
            
            let centerXConstraint = crossHairLabel.centerXAnchor.constraint(
                equalTo: overlayView.centerXAnchor)
            let centerYConstraint = crossHairLabel.centerYAnchor.constraint(
                equalTo: overlayView.centerYAnchor)
            
            centerXConstraint.isActive = true
            centerYConstraint.isActive = true
            
            
            //to make this overlayt view transparent so the user can access the camera controls
            overlayView.isUserInteractionEnabled = false
            
            //finally, set the overlay view
            imagePicker.cameraOverlayView = overlayView
        }
        else{
            imagePicker.sourceType = .photoLibrary
        }
        
        //set this controller to be the image picker's delegate
        imagePicker.delegate = self
        
        imagePicker.allowsEditing = true
        
        //now that we've set the imagePicker's source type and 
        //delegate, present it modally
        present(imagePicker, animated: true, completion: nil)
    }
    
    @IBAction func backgroundTapped(_ sender: UITapGestureRecognizer) {
        view.endEditing(true)
    }
    
    var item: Item!{
        didSet{
            navigationItem.title = item.name
        }
    }
    
    var imageStore: ImageStore!
    
    let numberFormatter: NumberFormatter = {
        let formatter = NumberFormatter()
        formatter.numberStyle = .decimal
        formatter.minimumFractionDigits = 2
        formatter.maximumFractionDigits = 2
        return formatter
    }()
    
    let dateFormatter: DateFormatter = {
        let formatter = DateFormatter()
        formatter.dateStyle = .medium
        formatter.timeStyle = .none
        return formatter
    }()
    
    var flexibleSpaceBarButton: UIBarButtonItem!
    var removePictureBarButton: UIBarButtonItem!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        flexibleSpaceBarButton = UIBarButtonItem(
            barButtonSystemItem: .flexibleSpace, target: nil, action: nil)
        removePictureBarButton = UIBarButtonItem(
            barButtonSystemItem: .trash, target: self, action: #selector(removePicture(_:)))
        
        //locate the toolbar at the bottom and add the bar buttons to it
        for subview in self.view.subviews{
            if let toolBar = subview as? UIToolbar{
                //get the items displayed in the toolbar
                var toolBarItems = toolBar.items
                
                toolBarItems?.append(flexibleSpaceBarButton)
                toolBarItems?.append(removePictureBarButton)
                toolBar.setItems(toolBarItems, animated: true)
                break
            }
        }
    }
    
    @IBAction func removePicture(_ sender: UIBarButtonItem){
        imageStore.deleteImage(forKey: item.itemKey)
        imageView.image = nil
        removePictureBarButton.isEnabled = false
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        nameField.text = item.name
        serialNumberField.text = item.serialNumber
        valueField.text = numberFormatter.string(from: NSNumber(value: item.valueInDollars))
        dateLabel.text = dateFormatter.string(from: item.dateCreated)
        
        //get the item key
        let key = item.itemKey
        
        //if there is an associated image with the item
        //display it on the image view
        //the first function called here returns an optional
        let imageToDisplay = imageStore.image(forKey: key)
        imageView.image = imageToDisplay
        
        //choosing whether to display the delete bar button item or not
        removePictureBarButton.isEnabled = (imageView.image != nil)
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        
        //clear first responder status
        view.endEditing(true)
        
        //"Save" changes to item
        item.name = nameField.text ?? ""
        item.serialNumber = serialNumberField.text
        
        if let valueText = valueField.text,
            let value = numberFormatter.number(from: valueText) {
            item.valueInDollars = value.intValue
        }
        else{
            item.valueInDollars = 0
        }
    }
    
    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        textField.resignFirstResponder()
        return true
    }
    
    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [String : Any]) {
        //get EDITED picked image from info dictionary
        let image = info[UIImagePickerControllerEditedImage] as! UIImage
        
        //store the image in the ImageStore for the item's key
        imageStore.setImage(image, forKey: item.itemKey)
        
        //put that image on the screen in the image view
        imageView.image = image
        
        //take image picker off the screen - you must call this 
        //dismiss method
        dismiss(animated: true, completion: nil)
        
        //enabling the remove image bar button item because we now have an image to remove
        removePictureBarButton.isEnabled = true
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        switch segue.identifier{
        case "changeDate"?:
            let dateCreatedViewController = segue.destination as! DateCreatedViewController
            dateCreatedViewController.item = item
        default:
            preconditionFailure("Unexpected segue identifier.")
        }
    }
}
