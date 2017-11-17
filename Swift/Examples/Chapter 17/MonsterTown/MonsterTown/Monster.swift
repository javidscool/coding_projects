//
//  Monster.swift
//  MonsterTown
//
//  Created by Javid Shamloo on 2/15/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import Foundation

class Monster{
    static let isTerrifying = true
    class var spookyNoise: String {
        return "Grrr"
    }
    
    var town: Town?
    var name: String
    
    var victimPool: Int{
        get{
            return town?.population ?? 0
        }
        set(newVictimPool){
            town?.population = newVictimPool
        }
    }
    
    init?(town: Town?, monsterName: String){
        
        let trimmedMonsterName = monsterName.trimmingCharacters(in: .whitespaces)
        
        guard !trimmedMonsterName.isEmpty else{
            return nil
        }
        
        self.town = town
        name = trimmedMonsterName
    }
    
    func terrorizeTown(){
        if town != nil{
            print("\(name) is terrorizing a town!")
        }
        else{
            print("\(name) hasn't found a town to terrorize yet...")
        }
    }
}
