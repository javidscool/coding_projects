//: Playground - noun: a place where people can play

import Cocoa

var str = "Hello, playground"

class GreekGod{
    var name: String
    
    init(name: String){
        self.name = name
    }
}

let hecate = GreekGod(name: "Hecate")
let anotherHecate = hecate
anotherHecate.name = "Another God"

print(hecate.name)
print(anotherHecate.name)

struct Pantheon{
    var chiefGod: GreekGod
}

let pantheon = Pantheon(chiefGod: hecate)
let zeus = GreekGod(name: "Zeus")
zeus.name = "Zeus Jr."
print(zeus.name)

print(pantheon.chiefGod.name)
let greekPantheon = pantheon
hecate.name = "Trivia"
print(greekPantheon.chiefGod.name)

let athena = GreekGod(name: "Athena")

let gods = [athena, hecate, zeus]
let godsCopy = gods

gods.last?.name = "Jupiter"

print()

for god in gods{
    print(god.name)
}

for god in godsCopy{
    print(god.name)
}





