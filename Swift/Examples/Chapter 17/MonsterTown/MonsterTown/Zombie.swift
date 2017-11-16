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
    
    var walksWithLimp: Bool
    private(set) var isFallingApart: Bool
    
    init?(limp: Bool, fallingApart: Bool, town: Town?, monsterName: String){
        
        walksWithLimp = limp
        isFallingApart = fallingApart
        super.init(town: town, monsterName: monsterName)
    }
    
    convenience init?(limp: Bool, fallingApart: Bool){
        self.init(limp: limp, fallingApart: fallingApart, town: nil, monsterName: "Fred")
        if walksWithLimp{
            print("Zombie has a bad knee")
        }
    }
    
    convenience override init?(town: Town?, monsterName: String){
        self.init(limp: false, fallingApart: false, town: town, monsterName: monsterName)
    }
    
    deinit {
        print("Zombie named \(name) is no longer with us")
    }
    
    final override func terrorizeTown() {
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
