//: Playground - noun: a place where people can play

import Cocoa



struct Point: Comparable{
    let x: Int
    let y: Int
    
    static func ==(lhs: Point, rhs: Point) -> Bool{
        return (lhs.x == rhs.x) && (lhs.y == rhs.y)
    }
    
    static func <(lhs: Point, rhs: Point) -> Bool{
        
        //platinum challenge
        let lhsDist = sqrt(pow((Double(lhs.x) - 0.0), 2) + pow((Double(lhs.y) - 0.0), 2))
        let rhsDist = sqrt(pow((Double(rhs.x) - 0.0), 2) + pow((Double(rhs.y) - 0.0), 2))
        
        return (lhsDist < rhsDist)
    }
    
    //bronze challenge
    static func +(lhs: Point, rhs: Point) -> Point{
        return Point(x: lhs.x + rhs.x, y: lhs.y + rhs.y)
    }
}


let a = Point(x: 3, y: 4)
let b = Point(x: 3, y: 4)
let c = Point(x: 2, y: 6)
let d = Point(x: 3, y: 7)

let abEqual = (a == b)
let abNotEqual = (a != b)

let cdEqual = (c == d)
let cLessThanD = (c < d)

let cLessThanEqualD = (c <= d)
let cGreaterThanD = (c > d)
let cGreaterThanEqualD = (c >= d)

let aPlusb = a + b
print("a + b is: \(aPlusb.x), \(aPlusb.y)")

let e = Point(x: 3, y: 3)
let f = Point(x: 2, y: 5)

let eGreaterf = (e > f)
let eLessf = (e < f)
let eEqualf = (e == f)


infix operator +++

func +++(lhs: Person, rhs: Person){
    lhs.spouse = rhs
    rhs.spouse = lhs
}

class Person : Equatable{
    var name: String
    var age: Int
    weak var spouse: Person?
    
    init(name: String, age: Int, spouse: Person?){
        self.name = name
        self.age = age
        self.spouse = spouse
    }
    
    static func==(lhs: Person, rhs: Person) -> Bool{
        return lhs.name == rhs.name
    }
}

let matt = Person(name: "Matt", age: 24, spouse: nil)
let drew = Person(name: "Drew", age: 32, spouse: nil)

matt +++ drew
print(matt.spouse?.name)
print(drew.spouse?.name)

//gold challenge
var personArray = [Person]()
personArray.append(matt)
personArray.append(drew)
print("index of drew is: \(personArray.index(of: drew)!)")