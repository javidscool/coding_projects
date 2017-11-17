//
//  PhotoStore.swift
//  Photorama
//
//  Created by Javid Shamloo on 3/1/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import Foundation
import UIKit
import CoreData

//I did the bronze and silver challenges for chapter 20
//I did the silver challenge for chapter 21...didn't feel like doing the gold challenge
//I did the bronze challenge for chapter 22
//I did the silver challenge for chapter 23

enum ImageResult {
    case success(UIImage)
    case failure(Error)
}

enum TagsResult {
    case success([Tag])
    case failure(Error)
}

enum PhotoError: Error {
    case imageCreationError
}

enum PhotosResult {
    case success([Photo])
    case failure(Error)
}

enum PhotoType {
    case interestingPhotos
    case recentPhotos
}

class PhotoStore {
    
    let imageStore = ImageStore()
    
    let persistentContainer: NSPersistentContainer = {
        let container = NSPersistentContainer(name: "Photorama")
        container.loadPersistentStores { (description, error) in
            if let error = error{
                print("Error setting up Core Data: \(error)")
            }
        }
        return container
    }()
    
    let session: URLSession = {
        let config =
            URLSessionConfiguration.default
        return URLSession(configuration: config)
    }()
    
    func fetchPhotos(photoType: PhotoType, completion: @escaping (PhotosResult) -> Void){
        
        var url: URL
        switch photoType{
        case .interestingPhotos:
            url = FlickrAPI.interestingPhotosURL
        case .recentPhotos:
            url = FlickrAPI.recentPhotosURL
        }
        
        let request = URLRequest(url: url)
        let task = session.dataTask(with: request){
            (data, response, error) -> Void in
            
            if let httpStatus = response as? HTTPURLResponse{
                //check for http errors
                print("fetchPhotos")
                print("Status code is: \(httpStatus.statusCode)")
                print("Header fields are: \(httpStatus.allHeaderFields)")
            }
            
            self.processPhotosRequest(data: data, error: error) {
                (result) in
                
                OperationQueue.main.addOperation {
                    completion(result)
                }
            }
        }
        
        task.resume()
    }
    
    private func processPhotosRequest(data: Data?,
                                      error: Error?,
                                      completion: @escaping (PhotosResult) -> Void) {
        guard let jsonData = data else{
            completion(.failure(error!))
            return
        }
        
        persistentContainer.performBackgroundTask {
            (context) in
            
            let result = FlickrAPI.photos(fromJSON: jsonData, into: context)
            
            do{
                try context.save()
            }
            catch{
                print("Error saving to Core Data: \(error)")
                completion(.failure(error))
                return
            }
            
            switch result{
            case let .success(photos):
                let photoIDs = photos.map{ return $0.objectID }
                
                let viewContext = self.persistentContainer.viewContext
                
                let viewContextPhotos = photoIDs.map{
                    return viewContext.object(with: $0)
                } as! [Photo]
                
                completion(.success(viewContextPhotos))
            case .failure:
                completion(result)
            }
        }
    }
    
    func fetchImage(for photo: Photo, completion: @escaping (ImageResult) -> Void) {
        
        guard let photoKey = photo.photoID else {
            preconditionFailure("Photo expected to have a photoID.")
        }
        if let image = imageStore.image(forKey: photoKey) {
            OperationQueue.main.addOperation {
                completion(.success(image))
            }
        }
        
        guard let photoURL = photo.remoteURL else{
            preconditionFailure("Photo expected to have a remoteURL.")
        }
        let request = URLRequest(url: photoURL as URL)
        
        let task = session.dataTask(with: request) {
            (data, response, error) -> Void in
            
            if let httpStatus = response as? HTTPURLResponse{
                //check for http errors
                print("fetchImage")
                print("Status code is: \(httpStatus.statusCode)")
                print("Header fields are: \(httpStatus.allHeaderFields)")
            }
            
            let result = self.processImageRequest(data: data,
                                                  error: error)
            
            if case let .success(image) = result {
                self.imageStore.setImage(image, forKey: photoKey)
            }
            
            OperationQueue.main.addOperation {
                completion(result)
            }
        }
        
        task.resume()
    }
    
    private func processImageRequest(data: Data?, error: Error?) -> ImageResult {
        guard let imageData = data,
            let image = UIImage(data: imageData) else {
            
            //we couldn't create an image
            if data == nil {
                return .failure(error!)
            }
            else{
                return .failure(PhotoError.imageCreationError)
            }
        }
        
        return .success(image)
    }
    
    //this function deletes all photos in core data so we can reload with new photos
    func deleteAllPhotos(){
        let fetchRequest: NSFetchRequest<Photo> = Photo.fetchRequest()
        let viewContext = persistentContainer.viewContext
        viewContext.perform {
            do{
                let allPhotos = try viewContext.fetch(fetchRequest)
                
                for photo: AnyObject in allPhotos{
                    viewContext.delete(photo as! NSManagedObject)
                }
                
                try viewContext.save()
            }
            catch{
                print("Cannot Delete Photos From Core Data!")
            }
        }
    }
    
    func fetchAllPhotos(completion: @escaping (PhotosResult) -> Void){
        let fetchRequest: NSFetchRequest<Photo> = Photo.fetchRequest()
        let sortByDateTaken = NSSortDescriptor(key: #keyPath(Photo.dateTaken), ascending: true)
        
        fetchRequest.sortDescriptors = [sortByDateTaken]
        
        let viewContext = persistentContainer.viewContext
        viewContext.perform {
            do{
                let allPhotos = try viewContext.fetch(fetchRequest)
                completion(.success(allPhotos))
            }
            catch{
                completion(.failure(error))
            }
        }
    }
    
    func fetchFavorites(isFav: Bool, completion: @escaping (PhotosResult) -> Void){
        let fetchRequest: NSFetchRequest<Photo> = Photo.fetchRequest()
        
        //set the sort descriptor
        let sortByDateTaken = NSSortDescriptor(key: #keyPath(Photo.dateTaken), ascending: true)
        fetchRequest.sortDescriptors = [sortByDateTaken]
        
        //set the predicate
        //we make it optional because if there is no predicate we get all the photos,
        //which is what we want because we do NOT want to get all photos minus favs
        var predicate: NSPredicate?
        
        if isFav{
            predicate = NSPredicate(format: "\(#keyPath(Photo.isFavorite)) == true")
        }
        
        fetchRequest.predicate = predicate
        
        //run this request
        let viewContext = persistentContainer.viewContext
        viewContext.perform {
            do{
                let favPhotos = try viewContext.fetch(fetchRequest)
                print("favorites count: \(favPhotos.count)")
                completion(.success(favPhotos))
            }
            catch{
                completion(.failure(error))
            }
        }

    }
    
    func fetchAllTags(completion: @escaping (TagsResult) -> Void){
        let fetchRequest: NSFetchRequest<Tag> = Tag.fetchRequest()
        let sortByName = NSSortDescriptor(
            key: #keyPath(Tag.name), ascending: true)
        fetchRequest.sortDescriptors = [sortByName]
        
        let viewContext = persistentContainer.viewContext
        viewContext.perform {
            do{
                let allTags = try fetchRequest.execute()
                completion(.success(allTags))
            }
            catch{
                completion(.failure(error))
            }
        }
    }
    
    func saveContextIfNeeded(){
        let context = persistentContainer.viewContext
        
        //if we've changed something in our context, like the view count on a photo
        //save the changes
        if context.hasChanges{
            do{
                try context.save()
            }
            catch{
                print("Cannot save context!")
            }
        }
    }
}
