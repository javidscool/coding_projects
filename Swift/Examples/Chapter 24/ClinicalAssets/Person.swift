//
//  Person.swift
//  ClinicalAssets
//
//  Created by Javid Shamloo on 2/7/16.
//  Copyright © 2016 BigNerdRanch. All rights reserved.
//

import Foundation

class Person: CustomStringConvertible{
    let name: String
    let accountant = Accountant()
    var assets = [Asset]()
    
    var description: String{
        return "Person(\(name))"
    }
    
    init(name: String){
        self.name = name
        
        accountant.netWorthChangedHandler = {
            [weak self] netWorth in
            self?.netWorthDidChange(to: netWorth)
            return
        }
    }
    
    deinit{
        print("\(self) is being deallocated")
    }
    
    func takeOwnership(of asset: Asset){
        
        //silver challenge
        if asset.owner == nil{
        
            accountant.gained(asset) {
                asset.owner = self
                assets.append(asset)
            }
        }
        else{
            print("SOMEONE ELSE ALREADY OWNS THIS ASSET")
        }
    }
    
    //bronze challenge
    func relinquish(asset: Asset){
        accountant.lost(asset){
            let result = assets.index(where: {(myAsset) -> Bool in
                myAsset.name == asset.name
            })
            asset.owner = nil
            assets.remove(at: result!)
        }
    }
    
    func netWorthDidChange(to netWorth: Double){
        print("The net worth of \(self) is now \(netWorth)")
    }
    
    func useNetWorthChangedHandler(_ handler: @escaping (Double) -> Void){
        accountant.netWorthChangedHandler = handler
    }
}
