//
//  Zombie.swift
//  MonsterTown
//
//  Created by Javid Shamloo on 2/15/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import Foundation

class Zombie: Monster{
    
    override class var spookyNoise: String {
        return "Brains..."
    }
    
    var walksWithLimp = true
    private(set) var isFallingApart = false
    
    override func terrorizeTown() {
        if !isFallingApart{
            town?.changePopulation(by: -10)
        }
        else{
            print("zombie is injured")
        }
        super.terrorizeTown()
    }
    
    func injureZombie(){
        isFallingApart = true
    }
    
    func changeName(name: String, walksWithLimp: Bool){
        self.name = name
        self.walksWithLimp = walksWithLimp
    }
}
