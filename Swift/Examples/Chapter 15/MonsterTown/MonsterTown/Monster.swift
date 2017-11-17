//
//  Monster.swift
//  MonsterTown
//
//  Created by Javid Shamloo on 2/15/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import Foundation

class Monster{
    var town: Town?
    var name = "Monster"
    
    func terrorizeTown(){
        if town != nil{
            print("\(name) is terrorizing a town!")
        }
        else{
            print("\(name) hasn't found a town to terrorize yet...")
        }
    }
}
