//
//  Town.swift
//  MonsterTown
//
//  Created by Javid Shamloo on 2/15/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import Foundation

struct Town{
    
    var mayor: Mayor = Mayor()
    
    static let region = "South"
    var population = 5422 {
        didSet(oldPopulation){
            //bronze challenge
            if population < oldPopulation{
                print("The population has changed to \(population) from \(oldPopulation)")
                mayor.makeStatement(attacked: true)
            }
        }
    }
    var numberOfStoplights = 4
    
    enum Size {
        case small
        case medium
        case large
    }
    
    var townSize: Size {
        get {
            switch self.population{
            case 0...10_000:
                return Size.small
            case 10_001...100_000:
                return Size.medium
            default:
                return Size.large
            }
        }
    }
    
    func printTownDescription(){
        print("Population: \(population), number of stoplights: \(numberOfStoplights)")
    }
    
    mutating func changePopulation(by amount: Int){
        population += amount
    }
}
