//
//  PhotosViewController.swift
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

class PhotosViewController : UIViewController, UICollectionViewDelegate {
    
    @IBOutlet var collectionView: UICollectionView!
    
    @IBAction func setInteresting(_ sender: UIBarButtonItem) {
        self.photoType = .interestingPhotos
        
        //remove all the photos currenly in core data and our collection view
        self.photoDataSource.photos.removeAll()
        self.store.deleteAllPhotos()
        
        store.fetchPhotos(photoType: self.photoType){
            (photosResult) -> Void in
            
            self.updateDataSource()
        }
    }
    
    @IBAction func setRecent(_ sender: UIBarButtonItem) {
        self.photoType = .recentPhotos
        
        //remove all the photos currenly in core data and our collection view
        self.photoDataSource.photos.removeAll()
        self.store.deleteAllPhotos()
        
        store.fetchPhotos(photoType: self.photoType){
            (photosResult) -> Void in
            
            self.updateDataSource()
        }
    }
    
    var store: PhotoStore!
    let photoDataSource = PhotoDataSource()
    
    var photoType: PhotoType = .interestingPhotos
    
    var favoritesSegmentedControl: UISegmentedControl!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        collectionView.dataSource = photoDataSource
        collectionView.delegate = self
        
        updateDataSource()
        
        store.fetchPhotos(photoType: self.photoType){
            (photosResult) -> Void in
            
            self.updateDataSource()
            self.refreshCollection()
        }
        
        
        //set up favorites segmented control
        favoritesSegmentedControl = UISegmentedControl(items: ["All Photos", "Favorites"])
        favoritesSegmentedControl.backgroundColor = UIColor.white.withAlphaComponent(0.5)
        
        favoritesSegmentedControl.selectedSegmentIndex = 0
        favoritesSegmentedControl.translatesAutoresizingMaskIntoConstraints = false
        self.view.addSubview(favoritesSegmentedControl)
        
        //set up segmented control constraints
        let bottomConstraint =
            favoritesSegmentedControl.bottomAnchor.constraint(
                equalTo: self.bottomLayoutGuide.topAnchor, constant: -48)
        let leadingConstraint =
            favoritesSegmentedControl.leadingAnchor.constraint(
                equalTo: self.view.layoutMarginsGuide.leadingAnchor)
        let trailingConstraint =
            favoritesSegmentedControl.trailingAnchor.constraint(
                equalTo: self.view.layoutMarginsGuide.trailingAnchor)
        
        bottomConstraint.isActive = true
        leadingConstraint.isActive = true
        trailingConstraint.isActive = true
        
        //add action event
        favoritesSegmentedControl.addTarget(self,
                                            action: #selector(toggleList(_:)),
                                            for: .valueChanged)
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        refreshCollection()
    }
    
    func toggleList(_ sender: AnyObject){
        refreshCollection()
    }
    
    func refreshCollection(){
        
        var isFavSelected: Bool
        if favoritesSegmentedControl.selectedSegmentIndex > 0{
            isFavSelected = true
        }
        else{
            isFavSelected = false
        }
        
        store.fetchFavorites(isFav: isFavSelected){
            (photosResult) in
            
            switch photosResult{
            case let .success(photos):
                self.photoDataSource.photos = photos
            case .failure:
                self.photoDataSource.photos.removeAll()
            }
            self.collectionView.reloadSections(IndexSet(integer: 0))
        }
    }
    
    func collectionView(_ collectionView: UICollectionView,
                        willDisplay cell: UICollectionViewCell,
                        forItemAt indexPath: IndexPath) {
        
        let photo = photoDataSource.photos[indexPath.row]
        
        //download the image data, which could take some time
        store.fetchImage(for: photo){
            (result) -> Void in
            
            //the index path for the photo might have changed between
            //the time the request started and finished, so find the most
            //recent index path
            
            guard let photoIndex =
                self.photoDataSource.photos.index(of: photo),
                case let .success(image) = result else{
                    return
            }
            
            let photoIndexPath = IndexPath(item: photoIndex, section:0)
            
            //when the request finishes, only update the cell if it's still
            //visible
            if let cell = self.collectionView
                .cellForItem(at: photoIndexPath)
                as? PhotoCollectionViewCell{
                cell.update(with: image)
            }
        }
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        switch segue.identifier{
        case "showPhoto"?:
            if let selectedIndexPath = collectionView.indexPathsForSelectedItems?.first {
                
                let photo = photoDataSource.photos[selectedIndexPath.row]
                let destinationVC = segue.destination as! PhotoInfoViewController
                destinationVC.photo = photo
                destinationVC.store = store
            }
        default:
            preconditionFailure("Unexpected segue identifier.")
        }
    }
    
    private func updateDataSource(){
        store.fetchAllPhotos {
            (photosResult) in
            
            switch photosResult {
            case let .success(photos):
                self.photoDataSource.photos = photos
            case .failure:
                self.photoDataSource.photos.removeAll()
            }
            self.collectionView.reloadSections(IndexSet(integer: 0))
        }
    }
}

extension PhotosViewController : UICollectionViewDelegateFlowLayout{
    
    //compute the size of the specified item's cell
    func collectionView(_ collectionView: UICollectionView,
                        layout collectionViewLayout: UICollectionViewLayout,
                        sizeForItemAt indexPath: IndexPath) -> CGSize {
        
        let collectionViewWidth = collectionView.bounds.size.width
        let numberOfItemsPerRow: CGFloat = 4
        
        //get the width that each cell should be
        let itemWidth = collectionViewWidth / numberOfItemsPerRow
        
        return CGSize(width: itemWidth, height: itemWidth)
    }
    
    //notify the container that the item width trait changed
    override func willTransition(to newCollection: UITraitCollection,
                                 with coordinator: UIViewControllerTransitionCoordinator) {
        collectionView.reloadData()
    }
    
}
