//
//  Vampire.swift
//  MonsterTown
//
//  Created by Javid Shamloo on 3/14/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import Foundation

//silver challenge
class Vampire: Monster {
    
    var vampireThrall = [Vampire]()
    
    override func terrorizeTown() {
        if (town?.population)! > 0{
            town?.changePopulation(by: -1)
            
            vampireThrall.append(Vampire())
            
            if (town?.population)! < 0 {
                town?.population = 0
            }
            
            super.terrorizeTown()
        }
    }
}
