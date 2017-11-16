//
//  main.swift
//  ClinicalAssets
//
//  Created by Javid Shamloo on 2/7/16.
//  Copyright © 2016 BigNerdRanch. All rights reserved.
//

import Foundation

var bob: Person? = Person(name: "Bob")
print("created \(bob)")

var laptop: Asset? = Asset(name: "Shiny Laptop", value: 1500.0)
var hat: Asset? = Asset(name: "Cowboy Hat", value: 175.0)
var backpack: Asset? = Asset(name: "Blue Backpack", value: 45.0)

bob?.useNetWorthChangedHandler { netWorth in
    print("Bob's net worth is now \(netWorth)")
}
bob?.takeOwnership(of: laptop!)
bob?.takeOwnership(of: hat!)

bob?.relinquish(asset: laptop!)

var javid: Person? = Person(name: "Javid")
javid?.useNetWorthChangedHandler { netWorth in
    print("Javid's net worth is now \(netWorth)")
}
javid?.takeOwnership(of: hat!)

print("While Bob is alive, hat's owner is \(hat!.owner?.name)")
bob = nil
print("the bob variable is now \(bob)")
print("After Bob is deallocated, hat's owner is \(hat!.owner?.name)")

laptop = nil
hat = nil
backpack = nil
