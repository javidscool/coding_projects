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
myTown.changePopulation(by: 1_000_000)
print("Size: \(myTown.townSize)")

let fredTheZombie = Zombie()
fredTheZombie.town = myTown
fredTheZombie.terrorizeTown()
fredTheZombie.town?.printTownDescription()

print("Victim pool: \(fredTheZombie.victimPool)")
fredTheZombie.victimPool = 500
print("Victim pool: \(fredTheZombie.victimPool); population: \(fredTheZombie.town?.population)")
print(Zombie.spookyNoise)
if Zombie.isTerrifying {
    print("Run away from the terrifying thing!")
}

fredTheZombie.injureZombie()
fredTheZombie.terrorizeTown()

fredTheZombie.town?.mayor.printAnxiety()



