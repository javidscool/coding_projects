//
//  TagDataSource.swift
//  Photorama
//
//  Created by Javid Shamloo on 3/4/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import UIKit
import CoreData

class TagDataSource: NSObject, UITableViewDataSource {
    var tags: [Tag] = []
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return tags.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        
        let cell = tableView.dequeueReusableCell(
            withIdentifier: "UITableViewCell", for: indexPath)
        
        let tag = tags[indexPath.row]
        cell.textLabel?.text = tag.name
        
        cell.accessibilityHint = "Double tap to toggle selected"
        
        return cell
    }
}
