//: Playground - noun: a place where people can play

import Cocoa

var groceryBag: Set = ["Apples", "Oranges", "Pineapples"]
//groceryBag.insert("Apples")
//groceryBag.insert("Oranges")
//groceryBag.insert("Bananas")

/*
for food in groceryBag{
    print(food)
}
*/

let hasBananas = groceryBag.contains("Bananas")

var friendsGroceryBag: Set = ["Bananas", "Cereal", "Milk", "Oranges"]
let commonGroceryBag = groceryBag.union(friendsGroceryBag)

for food in commonGroceryBag{
    print(food)
}

let roomatesGroceryBag = Set(["Apples", "Bananas", "Cereal", "Toothpaste"])
let itemsToReturn = commonGroceryBag.intersection(roomatesGroceryBag)

print("Items to return")
for itemReturn in itemsToReturn{
    print(itemReturn)
}

let secondBag = Set(["Berries", "Yogurt"])
//checking to see if the two bags are disjoint (don't contain any of the same items)
let disjoint = secondBag.isDisjoint(with: commonGroceryBag)
print(disjoint)


//bronze challenge
let myCities = Set(["Atlanta", "Chicago", "Jacksonville", "New York", "San Francisco"])
let yourCities = Set(["Chicago", "San Francisco", "Jacksonville"])

if myCities.isSuperset(of: yourCities){
    print("we have a superset")
}


//silver challenge
var groceryBag2: Set = ["Apples", "Oranges", "Pineapples"]

var friendsGroceryBag2: Set = ["Bananas", "Cereal", "Milk", "Oranges"]
groceryBag2.formUnion(friendsGroceryBag2)
print("\n")
for food in groceryBag2{
    print(food)
}


var roomatesGroceryBag2 = Set(["Apples", "Bananas", "Cereal", "Toothpaste"])
roomatesGroceryBag2.formIntersection(groceryBag2)
print("\n")
for food in roomatesGroceryBag2{
    print(food)
}



