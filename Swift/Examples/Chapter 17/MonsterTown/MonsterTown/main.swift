//
//  main.swift
//  MonsterTown
//
//  Created by Javid Shamloo on 2/15/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import Foundation

var myTown: Town? = Town(population: 10, stoplights: 6)
myTown?.printTownDescription()
myTown?.changePopulation(by: 500)
myTown?.changePopulation(by: 1_000_000)
print("Size: \(myTown?.townSize)")

var fredTheZombie: Zombie? = Zombie(limp: false, fallingApart: false, town: myTown, monsterName: "Fred")
fredTheZombie?.terrorizeTown()
fredTheZombie?.town?.printTownDescription()

var convenientZombie = Zombie(limp: true, fallingApart: false)

print("Victim pool: \(fredTheZombie?.victimPool)")
fredTheZombie?.victimPool = 500
print("Victim pool: \(fredTheZombie?.victimPool); population: \(fredTheZombie?.town?.population)")
print(Zombie.spookyNoise)
if Zombie.isTerrifying {
    print("Run away from the terrifying thing!")
}

fredTheZombie?.injureZombie()
fredTheZombie?.terrorizeTown()

fredTheZombie = nil

if var javidTheZombie = Zombie(town: myTown, monsterName: "      "){
    print("We have a zombie!")
}
else{
    print("WE HAVE NIL!")
}
