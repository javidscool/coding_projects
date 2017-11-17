//
//  PhotoCollectionViewCell.swift
//  Photorama
//
//  Created by Javid Shamloo on 3/1/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import UIKit

class PhotoCollectionViewCell: UICollectionViewCell{
    @IBOutlet var imageView: UIImageView!
    @IBOutlet var spinner: UIActivityIndicatorView!
    
    var photoDescription: String?
    
    override var isAccessibilityElement: Bool{
        get{
            return true
        }
        set{
            super.isAccessibilityElement = newValue
        }
    }
    
    override var accessibilityLabel: String? {
        get{
            return photoDescription
        }
        set{
            //ignore attempts to set
        }
    }
    
    override var accessibilityTraits: UIAccessibilityTraits {
        get{
            return super.accessibilityTraits | UIAccessibilityTraitImage
        }
        set{
            //ignore attempts to set
        }
    }
    
    func update(with image: UIImage?){
        if let imageToDisplay = image{
            spinner.stopAnimating()
            imageView.image = imageToDisplay
        }
        else{
            spinner.startAnimating()
            imageView.image = nil
        }
    }
    
    override func awakeFromNib() {
        super.awakeFromNib()
        
        update(with: nil)
    }
    
    override func prepareForReuse() {
        super.prepareForReuse()
        
        update(with: nil)
    }
}
