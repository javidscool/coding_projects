//: Playground - noun: a place where people can play

import Cocoa

var population: Int = -55422
var message: String
var hasPostOffice: Bool = false

if population >= 0 && population < 10000{
    message = "\(population) is a small town!"
}
else if population >= 10000 && population < 50000{
    message = "\(population) is a medium town!"
}
else if population >= 50000{
    message = "\(population) is pretty big!"
}
else{
    message = "Can't have a negative population!"
}

//message = population < 10000 ? "\(population) is a small town!" : "\(population) is pretty big!"

print(message)

if !hasPostOffice{
    print("WHere do we buy stamps?")
}
