//
//  ItemStore.swift
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

class ItemStore {
    var allItems = [Item]()
    
    var highValueItems: [Item] {
        return allItems.filter{ $0.valueInDollars > 50 }
    }
    
    var otherItems: [Item] {
        return allItems.filter{ $0.valueInDollars <= 50 }
    }
    
    let itemArchiveURL: URL = {
        let documentsDirectory =
            FileManager.default.urls(
                for: .documentDirectory, in: .userDomainMask)
        
        let documentDirectory = documentsDirectory.first!
        
        return documentDirectory.appendingPathComponent(
            "items.archive")
    }()
    
    init(){
        if let archivedItems = NSKeyedUnarchiver.unarchiveObject(
            withFile: itemArchiveURL.path) as? [Item]{
            allItems += archivedItems
        }
    }
    
    @discardableResult func createItem() -> Item {
        let newItem = Item(random: true)
        
        allItems.append(newItem)
        
        return newItem
    }
    
    func removeItem(_ item: Item){
        if let index = allItems.index(of: item){
            allItems.remove(at: index)
        }
    }
    
    func moveItemAtIndex(from fromIndexPath: IndexPath, to toIndexPath: IndexPath){
        if fromIndexPath == toIndexPath{
            return
        }
        
        var fromItem: Item
        var toItem: Item
        
        //get the items from the proper computed property
        if fromIndexPath.section == 0{
            fromItem = self.highValueItems[fromIndexPath.row]
            toItem = self.highValueItems[toIndexPath.row]
        }
        else{
            fromItem = self.otherItems[fromIndexPath.row]
            toItem = self.otherItems[toIndexPath.row]
        }
        
        //next get the index of these items
        let fromIndex = allItems.index(of: fromItem)
        let toIndex = allItems.index(of: toItem)
        
        //now move the item
        let movedItem = allItems[fromIndex!]
        
        //remove item from array
        allItems.remove(at: fromIndex!)
        
        //insert item in array at new location
        allItems.insert(movedItem, at: toIndex!)
    }
    
    func saveChanges() -> Bool{
        print("Saving items to \(itemArchiveURL.path)")
        return NSKeyedArchiver.archiveRootObject(allItems,
                                                 toFile: itemArchiveURL.path)
    }
}
