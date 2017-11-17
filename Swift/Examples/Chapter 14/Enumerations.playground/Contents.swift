//: Playground - noun: a place where people can play

import Cocoa

enum Lightbulb{
    case on
    case off
    
    func surfaceTemp(forAmbientTemp ambient: Double) -> Double {
        switch self{
        case .on:
            return ambient + 150.0
            
        case .off:
            return ambient
        }
    }
    
    mutating func toggle(){
        switch self {
        case .on:
            self = .off
        case .off:
            self = .on
        }
    }
}

var bulb = Lightbulb.on
let ambientTemp = 77.0

var bulbTemp = bulb.surfaceTemp(forAmbientTemp: ambientTemp)
print(bulbTemp)

bulb.toggle()
bulbTemp = bulb.surfaceTemp(forAmbientTemp: ambientTemp)
print(bulbTemp)

enum ShapeDimensions {
    //point has no associated value - it's dimensionless
    case point
    
    //square's associated value is the length of one side
    case square(side: Double)
    
    //rectangle's associated value defines its width and height
    case rectangle(width: Double, height: Double)
    
    //silver challenge
    //right triangle's associated value defines the three sides
    case rightTriangle(base: Double, height: Double, hypot: Double)
    
    func area() -> Double{
        switch self {
        case .point:
            return 0
        case let .square(side: side):
            return side * side
        case let .rectangle(width: w, height: h):
            return w * h
        case let .rightTriangle(base: b, height: h, _):
            return ((b * h) / 2)
        }
    }
    
    //bronze challenge
    func perimeter() -> Double{
        switch self {
        case .point:
            return 0
        case let .square(side: side):
            return side * 4
        case let .rectangle(width: w, height: h):
            return (w * 2) + (h * 2)
        case let .rightTriangle(base: b, height: h, hypot: hy):
            return b + h + hy
        }
    }
    
}

var squareShape = ShapeDimensions.square(side: 10.0)
var rectShape = ShapeDimensions.rectangle(width: 5.0, height: 10.0)
var pointShape = ShapeDimensions.point
var triangleShape = ShapeDimensions.rightTriangle(base: 3, height: 4, hypot: 5)

print("square's area = \(squareShape.area())")
print("rectangle's area = \(rectShape.area())")
print("point's area = \(pointShape.area())")
print("right triangle's area = \(triangleShape.area())")

print("square's perimeter = \(squareShape.perimeter())")
print("rectangle's perimeter = \(rectShape.perimeter())")
print("point's perimeter = \(pointShape.perimeter())")
print("right triangle's perimeter = \(triangleShape.perimeter())")

enum FamilyTree{
    case noKnownParents
    
    indirect case oneKnownParent(name: String, ancestors: FamilyTree)
    
    indirect case twoKnownParents(
        fatherName: String, fatherAncestors: FamilyTree,
        motherName: String, motherAncestors: FamilyTree)
    
    
    func printFatherName(){
        switch self{
        case let .twoKnownParents(fatherName: fName, fatherAncestors: _, motherName: _, motherAncestors:_):
            print(fName)
        default:
            print("no known father")
        }
    }
 
}

let fredAncestors = FamilyTree.twoKnownParents(
    fatherName: "Fred Sr.",
    fatherAncestors: .oneKnownParent(name: "Beth", ancestors: .noKnownParents),
    motherName: "Marsha",
    motherAncestors: .noKnownParents)

print(fredAncestors)
print(fredAncestors.printFatherName())
