//: Playground - noun: a place where people can play

import Cocoa

var bucketList = ["a"]
var newItems = [ "b", "d", "e", "f", "g"]

//appending at once to array
bucketList += newItems

bucketList.insert("c", at: 0)
bucketList.remove(at: 1)
print(bucketList)
print(bucketList.count)
print(bucketList[0...2])

var otherList = ["c", "b", "d", "e", "f", "g"]
//checking array equality
let equal = (bucketList == otherList)


//bronze challenge
var toDoList = ["Take out garbage", "Pay bills", "Cross off finished items"]
if !(toDoList.isEmpty){
    print("we have items")
}


//silver challenge
var reverseList = [String]()
for item in toDoList {
    reverseList.insert(item, at: reverseList.startIndex)
}
print(reverseList)

var reverseList2 = Array(toDoList.reversed())
print(reverseList2)


//gold challenge
var newItems2 = [ "a", "b", "c", "d", "e"]
var indexOfTwo = newItems2.index(of: "c")
var indexOfFour = newItems2.index(indexOfTwo!, offsetBy: 2)
print(newItems2[indexOfFour])




