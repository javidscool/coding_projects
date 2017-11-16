//
//  Town.swift
//  MonsterTown
//
//  Created by Javid Shamloo on 2/15/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import Foundation

struct Town{
    var population = 5_422
    var numberOfStoplights = 4
    
    func printTownDescription(){
        print("Population: \(population), number of stoplights: \(numberOfStoplights)")
    }
    
    mutating func changePopulation(by amount: Int){
        population += amount
    }
}
