//
//  PhotoDataSource.swift
//  Photorama
//
//  Created by Javid Shamloo on 3/1/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import UIKit

//I did the bronze and silver challenges for chapter 20
//I did the silver challenge for chapter 21...didn't feel like doing the gold challenge
//I did the bronze challenge for chapter 22
//I did the silver challenge for chapter 23

class PhotoDataSource: NSObject, UICollectionViewDataSource{
    var photos = [Photo]()
    
    func collectionView(_ collectionView: UICollectionView, 	numberOfItemsInSection section: Int) -> Int {
        return photos.count
    }
    
    func collectionView(_ collectionView: UICollectionView,
                        cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        
        let identifier = "PhotoCollectionViewCell"
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: identifier,
                            for: indexPath) as! PhotoCollectionViewCell
        
        let photo = photos[indexPath.row]
        cell.photoDescription = photo.title
        
        return cell    
    }
    
}
