//
//  TodoList.swift
//  iTahDoodle
//
//  Created by Javid Shamloo on 2/8/16.
//  Copyright © 2016 BigNerdRanch. All rights reserved.
//

import UIKit

class TodoList: NSObject {
    
    //I DID THE BRONZE, SILVER, AND GOLD CHALLENGES
    
    fileprivate let fileURL: URL = {
        let documentDirectoryURLs = FileManager.default.urls(
            for: .documentDirectory, in: .userDomainMask)
        
        let documentDirectoryURL = documentDirectoryURLs.first!
        
        return documentDirectoryURL.appendingPathComponent("todolist.items")
    }()

    fileprivate var items: [String] = []
    
    override init(){
        super.init()
        loadItems()
    }
    
    func saveItems(){
        let itemsArray = items as NSArray
        
        print("Saving items to \(fileURL)")
        
        if !itemsArray.write(to: fileURL, atomically: true){
            print("Could not save to-do list.")
        }
    }
    
    func loadItems(){
        if let itemsArray = NSArray(contentsOf: fileURL) as? [String]{
            items = itemsArray
        }
    }
    
    func add(_ item: String){
        items.append(item)
        saveItems()
    }
    
    func remove(_ item: IndexPath){
        items.remove(at: item.row)
        saveItems()
    }
    
}

extension TodoList: UITableViewDataSource{
    
    //create a tuple to hold our cell information
    var getCellInfo: (cellClass: AnyClass, reuseIdentifier: String){
        return (UITableViewCell.self, "Cell")
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return items.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: getCellInfo.reuseIdentifier,
                                                 for: indexPath)
        
        let item = items[indexPath.row]
        
        cell.textLabel!.text = item
        
        return cell
    }
}
