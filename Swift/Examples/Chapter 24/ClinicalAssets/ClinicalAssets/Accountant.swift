//
//  Accountant.swift
//  ClinicalAssets
//
//  Created by Javid Shamloo on 2/7/16.
//  Copyright © 2016 BigNerdRanch. All rights reserved.
//

import Foundation

class Accountant{
    typealias NetWorthChanged = (Double) -> Void
    
    var netWorthChangedHandler: NetWorthChanged? = nil
    var netWorth: Double = 0.0{
        didSet{
            netWorthChangedHandler?(netWorth)
        }
    }
    
    func gained(_ asset: Asset, completion: () -> Void){
        netWorth += asset.value
        completion()
    }
    
    //bronze challenge
    func lost(_ asset: Asset, completion: () -> Void){
        netWorth -= asset.value
        completion()
    }
}
