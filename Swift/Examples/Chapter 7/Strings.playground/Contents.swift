//: Playground - noun: a place where people can play

import Cocoa

var mutablePlayground = "Hello, mutable playground"
mutablePlayground += "!"

for c: Character in mutablePlayground.characters{
    print("'\(c)'")
}

let oneCoolDude = "\u{1F60E}"
let aAcute = "\u{0061}\u{0301}"

let aAcutePrecomposed = "\u{00E1}"
print((aAcute == aAcutePrecomposed))
print("aAcute: \(aAcute.characters.count) aAcutePrecomposed: \(aAcutePrecomposed.characters.count)")

let start = mutablePlayground.startIndex
let end = mutablePlayground.index(start, offsetBy: 4)
let fifthCharacter = mutablePlayground[end]
let range = start...end
let firstFive = mutablePlayground[range]

/*
for scalar in mutablePlayground.unicodeScalars{
    print("\(scalar.value)")
}
*/


//bronze challenge
let empty = ""
let startEmpty = empty.startIndex
let endEmpty = empty.endIndex
if(startEmpty == endEmpty){
    print("we have an empty string")
}
if(empty.isEmpty){
    print("empty string!!!")
}


//silver challenge
let hello = "\u{0068}\u{0065}\u{006C}\u{006C}\u{006F}"
print(hello)
