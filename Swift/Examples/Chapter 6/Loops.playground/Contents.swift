//: Playground - noun: a place where people can play

import Cocoa

for i in 1...100 where i % 3 == 0{
    print(i)
}

var myFirstInt: Int = 0
for _ in 1...5{
    myFirstInt += 1
    print(myFirstInt)
}

/*
//game
var shields = 5
var blastersOverheating = false
var blasterFireCount = 0
var spaceDemonsDestroyed = 0

while shields > 0{
    
    if spaceDemonsDestroyed == 500 {
        print("You beat the game!")
        break
    }
    
    if blastersOverheating{
        print("Blasters are overheated!  Cooldown initiated.")
        sleep(5)
        print("Blasters ready to fire")
        sleep(1)
        blastersOverheating = false
        blasterFireCount = 0
    }
    
    if blasterFireCount > 100 {
        blastersOverheating = true
        continue
    }
    
    //fire blasters!
    print("Fire blasters")
    
    blasterFireCount += 1
    spaceDemonsDestroyed += 1
}
*/


//FIZZ BUZZ - Silver Challenge
for i in 1...300{
    if i % 3 == 0 && i % 5 == 0 {
        print("\(i) FIZZ BUZZ")
    }
    else if i % 3 == 0 {
        print("\(i) FIZZ")
    }
    else if i % 5 == 0 {
        print("\(i) BUZZ")
    }
    else{
        print(i)
    }
}

for i in 1...300{
    var fizz_buzz = ( (i % 3 == 0), (i % 5 == 0))
    switch fizz_buzz{
    case (true, true):
        print("\(i) FIZZ BUZZ")
    case (true, false):
        print("\(i) FIZZ")
    case (false, true):
        print("\(i) BUZZ")
    default:
        print(i)
    }
}
