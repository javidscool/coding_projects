//
//  ImageStore.swift
//  Homepwner
//
//  Created by Javid Shamloo on 2/28/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import UIKit

//I did the bronze and silver challenges for chapter 20
//I did the silver challenge for chapter 21...didn't feel like doing the gold challenge
//I did the bronze challenge for chapter 22
//I did the silver challenge for chapter 23

class ImageStore {
    let cache = NSCache<NSString, UIImage>()
    
    func setImage(_ image: UIImage, forKey key: String){
        cache.setObject(image, forKey: key as NSString)
        
        //create full URL for image
        let url = imageURL(forKey: key)
        
        //turn image into JPEG data
        if let data = UIImageJPEGRepresentation(image, 0.5){
            //write it to full URL
            let _ = try? data.write(to: url, options: [.atomic])
        }
    }
    
    func image(forKey key: String) -> UIImage?{
        if let existingImage = cache.object(forKey: key as NSString) {
            return existingImage
        }
        
        let url = imageURL(forKey: key)
        guard let imageFromDisk =
            UIImage(contentsOfFile: url.path) else{
                return nil
        }
        
        cache.setObject(imageFromDisk, forKey: key as NSString)
        return imageFromDisk
    }
    
    func deleteImage(forKey key: String){
        cache.removeObject(forKey: key as NSString)
        
        let url = imageURL(forKey: key)
        
        do{
            try FileManager.default.removeItem(at: url)
        }
        catch let deleteError{
            print("Error removing the image from disk: \(deleteError)")
        }
    }
    
    func imageURL(forKey key: String) -> URL{
        let documentsDirectories = FileManager.default.urls(
            for: .documentDirectory,
             in: .userDomainMask)
        
        let documentDirectory = documentsDirectories.first!
        
        return documentDirectory.appendingPathComponent(key)
    }
}
