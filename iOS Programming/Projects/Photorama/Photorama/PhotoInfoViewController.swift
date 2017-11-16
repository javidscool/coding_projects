//
//  PhotoInfoViewController.swift
//  Photorama
//
//  Created by Javid Shamloo on 3/1/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import UIKit

class PhotoInfoViewController: UIViewController {
    @IBOutlet var imageView: UIImageView!
    @IBOutlet var viewLabel: UILabel!
    @IBOutlet var bookmarkButton: UIBarButtonItem!
    
    @IBAction func toggleFavorite(_ sender: UIBarButtonItem) {
        //update the favorite status and bookmark button
        photo.isFavorite = !photo.isFavorite
        bookmarkButton.tintColor = photo.isFavorite ? UIColor.blue : UIColor.gray
        
        //save the changes to the store
        store.saveContextIfNeeded()
    }
    
    var photo: Photo! {
        didSet{
            navigationItem.title = photo.title
        }
    }
    var store: PhotoStore!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        imageView.accessibilityLabel = photo.title
        
        store.fetchImage(for: photo) {
            (result) -> Void in
            switch result{
            case let .success(image):
                self.imageView.image = image
            case let .failure(error):
                print("Error fetching image for photo: \(error)")
            }
        }
        
        //update photo count and save changes
        photo.viewCount += 1
        store.saveContextIfNeeded()
        
        //create label that displays photo count
        if photo.viewCount == 1{
           viewLabel.text = "\(photo.viewCount) view"
        }
        else{
            viewLabel.text = "\(photo.viewCount) views"
        }
        
        //setting up the bookmark button to state whether we have a fav or not
        bookmarkButton.tintColor = photo.isFavorite ? UIColor.blue : UIColor.gray
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        switch segue.identifier {
        case "showTags"?:
            let navController = segue.destination as! UINavigationController
            let tagController = navController.topViewController as! TagsViewController
            
            tagController.store = store
            tagController.photo = photo
        default:
            preconditionFailure("Unexpected segue identifier")
        }
    }
}
