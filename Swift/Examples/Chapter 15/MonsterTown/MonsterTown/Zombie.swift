//
//  Zombie.swift
//  MonsterTown
//
//  Created by Javid Shamloo on 2/15/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import Foundation

class Zombie: Monster{
    var walksWithLimp = true
    
    override func terrorizeTown() {
        //bronze challenge
        if (town?.population)! > 0{
            town?.changePopulation(by: -10)
            
            if (town?.population)! < 0 {
                town?.population = 0
            }
            
            super.terrorizeTown()
        }
    }
    
    func changeName(name: String, walksWithLimp: Bool){
        self.name = name
        self.walksWithLimp = walksWithLimp
    }
}
