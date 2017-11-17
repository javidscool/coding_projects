//
//  ItemCell.swift
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

class ItemCell : UITableViewCell {
    
    @IBOutlet var nameLabel: UILabel!
    @IBOutlet var serialNumberLabel: UILabel!
    @IBOutlet var valueLabel: UILabel!
    
    override func awakeFromNib() {
        super.awakeFromNib()
        
        nameLabel.adjustsFontForContentSizeCategory = true
        serialNumberLabel.adjustsFontForContentSizeCategory = true
        valueLabel.adjustsFontForContentSizeCategory = true
    }
    
    //override this to lay out our data with the color of the values
    override func layoutSubviews() {
        super.layoutSubviews()
        updateValueLabelTextColor()
    }
    
    func updateValueLabelTextColor(){
        if let text = valueLabel.text, !(text.isEmpty){
            //getting past the $ to the actual number
            let startIndex = text.index(text.startIndex, offsetBy: 1)
            if let value = Double(text.substring(from: startIndex)){
                if value <= 50{
                    valueLabel.textColor = UIColor.green
                }
                else{
                    valueLabel.textColor = UIColor.red
                }
            }
        }
    }
}
