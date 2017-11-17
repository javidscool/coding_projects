//
//  ItemsViewController.swift
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

class ItemsViewController : UITableViewController {
    
    var itemStore: ItemStore!
    var imageStore: ImageStore!
    
    @IBAction func addNewItem(_ sender: UIBarButtonItem){
        //create a new item and add it to the store
        let newItem = itemStore.createItem()
        
        //figure out what section this new item belongs to
        var section: Int
        if newItem.valueInDollars > 50{
            section = 0
            
            if let index = itemStore.highValueItems.index(of: newItem){
                let indexPath = IndexPath(row: index, section: section)
                
                //insert this new row into the table
                tableView.insertRows(at: [indexPath], with: .automatic)
            }
        }
        else{
            section = 1
            
            if let index = itemStore.otherItems.index(of: newItem){
                let indexPath = IndexPath(row: index, section: section)
                
                //insert this new row into the table
                tableView.insertRows(at: [indexPath], with: .automatic)
            }
        }
        
    }
    
    override func numberOfSections(in tableView: UITableView) -> Int {
        return 3
    }
    
    override func tableView(_ tableView: UITableView, titleForHeaderInSection section: Int) -> String? {
        switch section{
        case 0:
            return "More than $50"
        case 1:
            return "Less than or equal to $50"
        default:
            return nil //for "No more items!", which gets its own section
        }
    }
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        
        switch section{
        case 0:
            return itemStore.highValueItems.count
        case 1:
            return itemStore.otherItems.count
        default:
            //for "no more items"
            return 1
        }
    }
    
    override func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        if indexPath.section <= 1{
            return 80
        }
        else{
            return 44 //for "no more items" row
        }
    }
    
    override func tableView(_ tableView: UITableView,
                            cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        
        //create an instance of UITableViewCell, with default appearance
        let cell = tableView.dequeueReusableCell(
            withIdentifier: "ItemCell",
            for: indexPath) as! ItemCell
        
        
        
        if indexPath.section <= 1{
            //we first need to figure out which group of items we're working with
            //items greater than $50 or the other ones
            var items: [Item]
            switch indexPath.section{
            case 0:
                items = itemStore.highValueItems
            default:
                items = itemStore.otherItems
            }
            
            //set the text on the cell with the description of the item
            //that is at the nth index of items, where n = row this cell
            //will appear in on the tableview
            let item = items[indexPath.row]
            
            //configure the cell with the Item
            cell.nameLabel.text = item.name
            cell.serialNumberLabel.text = item.serialNumber
            cell.valueLabel.text = "$\(item.valueInDollars)"
            
            //change the font
            cell.nameLabel.font = UIFont.systemFont(ofSize: 20)
            cell.serialNumberLabel.font = UIFont.systemFont(ofSize: 20)
            cell.valueLabel.font = UIFont.systemFont(ofSize: 20)
        }
        else{
            //set up the "no more items" row
            cell.nameLabel.text = "No more items!"
            cell.serialNumberLabel.text = ""
            cell.valueLabel.text = ""
        }
        
        //change the background color to make the background image appear better
        cell.backgroundColor = UIColor.clear
        return cell
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        tableView.rowHeight = UITableViewAutomaticDimension
        tableView.estimatedRowHeight = 65
        
        //setting up the background image
        let imageView = UIImageView(image: UIImage(named: "background"))
        imageView.contentMode = .scaleAspectFit
        tableView.backgroundView = imageView
    }
    
    override func tableView(_ tableView: UITableView,
                            commit editingStyle: UITableViewCellEditingStyle,
                            forRowAt indexPath: IndexPath) {
        
        
        
        //if the table view is asking to commit a delete command...
        if editingStyle == .delete{
            
            var itemToDelete: Item
            
            if indexPath.section == 0{
                itemToDelete = itemStore.highValueItems[indexPath.row]
            }
            else if indexPath.section == 1{
                itemToDelete = itemStore.otherItems[indexPath.row]

            }
            else{
                //we can't delete the "no more items" row
                
                let noDeleteMessage = "You cannot delete this row"
                let alertController = UIAlertController(title: "",
                                                        message: noDeleteMessage,
                                                        preferredStyle: .alert)
                
                let okAction = UIAlertAction(title: "OK",
                                             style: .default,
                                             handler: nil)
                alertController.addAction(okAction)
                
                present(alertController, animated: true, completion: nil)
                
                return
            }
            
            //creating an alert controller to confirm deletion of row
            let title = "Delete \(itemToDelete.name)?"
            let message = "Are you sure you want to delete this item?"
            
            let ac = UIAlertController(title: title,
                                       message: message,
                                       preferredStyle: .actionSheet)
            
            let cancelAction = UIAlertAction(title: "Cancel",
                                             style: .cancel,
                                             handler: nil)
            ac.addAction(cancelAction)
            
            let deleteAction = UIAlertAction(title: "Delete",
                                             style: .destructive,
                                             handler: {
                                                (action) -> Void in
                
                if let index = self.itemStore.allItems.index(of: itemToDelete){
                    
                    let item = self.itemStore.allItems[index]
                    
                    //remove the item from the store
                    self.itemStore.removeItem(item)
                                                
                    //remove the item's image from the omage store
                    self.imageStore.deleteImage(forKey: item.itemKey)
                                                
                    //also remove that row from the table view with an animation
                    self.tableView.deleteRows(at: [indexPath],
                                              with: .automatic)
                }
            })
            ac.addAction(deleteAction)
            
            //present the alert controller
            present(ac, animated: true, completion: nil)
        }    
    }
    
    override func tableView(_ tableView: UITableView, canMoveRowAt indexPath: IndexPath) -> Bool {
        
        //make it so that we can't move the "no more items" row
        if indexPath.section == 2{
            return false
        }
        else{
            return true
        }
    }
    
    override func tableView(_ tableView: UITableView, targetIndexPathForMoveFromRowAt sourceIndexPath: IndexPath, toProposedIndexPath proposedDestinationIndexPath: IndexPath) -> IndexPath {
        
        //we don't need to worry about moving the "no more items" row because we take care 
        //of that in the canMoveRowAt function above
        
        var newIndexPath: IndexPath
        
        //if we're trying to move something to another section, make it stay in the same place
        if sourceIndexPath.section != proposedDestinationIndexPath.section{
            newIndexPath = sourceIndexPath
        }
        else{
            newIndexPath = proposedDestinationIndexPath
        }
        
        return newIndexPath
    }
    
    override func tableView(_ tableView: UITableView,
                            moveRowAt sourceIndexPath: IndexPath,
                            to destinationIndexPath: IndexPath) {
        
        //update the model
        itemStore.moveItemAtIndex(from: sourceIndexPath,
                                  to: destinationIndexPath)
    }
    
    override func prepare(
        for segue: UIStoryboardSegue,
        sender: Any?) {
        //if the triggered segue is the "showItem" segue
        switch segue.identifier{
        case "showItem"?:
            //figure out which row was just tapped
            if let selectedIndexPath = tableView.indexPathForSelectedRow{
                
                let detailViewController =
                    segue.destination as! DetailViewController
                detailViewController.imageStore = imageStore
                
                var selectedItem: Item
                if selectedIndexPath.section == 0{
                    selectedItem = itemStore.highValueItems[selectedIndexPath.row]
                    detailViewController.item = selectedItem
                }
                else if selectedIndexPath.section == 1{
                    selectedItem = itemStore.otherItems[selectedIndexPath.row]
                    detailViewController.item = selectedItem
                }
            }
        default:
            preconditionFailure("Unexpected seque identifier.")
        }
    }
    
    override func shouldPerformSegue(withIdentifier identifier: String, sender: Any?) -> Bool {
        
        var result: Bool = true
        
        if identifier == "showItem"{
            if let selectedIndexPath = tableView.indexPathForSelectedRow{
                if selectedIndexPath.section == 2{
                    result = false
                }
                else{
                    result = true
                }
            }
        }
        
        return result
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        tableView.reloadData()
    }
    
    //rename delete button to remove
    override func tableView(_ tableView: UITableView, titleForDeleteConfirmationButtonForRowAt indexPath: IndexPath) -> String? {
        return "Remove"
    }
    
    required init?(coder aDecoder: NSCoder){
        super.init(coder: aDecoder)
        
        navigationItem.leftBarButtonItem = editButtonItem
    }
}
