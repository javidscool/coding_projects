//
//  main.swift
//  MonsterTown
//
//  Created by Javid Shamloo on 2/15/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import Foundation

var myTown: Town = Town()
myTown.changePopulation(by: 500)

let fredTheZombie = Zombie()
fredTheZombie.town = myTown
fredTheZombie.town?.printTownDescription()
fredTheZombie.terrorizeTown()
fredTheZombie.town?.printTownDescription()


var myTown2: Town = Town()
myTown2.changePopulation(by: -5_417)
myTown2.printTownDescription()

let johnTheZombie = Zombie()
johnTheZombie.town = myTown2
johnTheZombie.terrorizeTown()
johnTheZombie.town?.printTownDescription()


let jazzyTheVampire = Vampire()
jazzyTheVampire.town = fredTheZombie.town
jazzyTheVampire.town?.printTownDescription()
jazzyTheVampire.terrorizeTown()
jazzyTheVampire.terrorizeTown()
jazzyTheVampire.terrorizeTown()
jazzyTheVampire.town?.printTownDescription()
print("Vampire count is: \(jazzyTheVampire.vampireThrall.count)")
