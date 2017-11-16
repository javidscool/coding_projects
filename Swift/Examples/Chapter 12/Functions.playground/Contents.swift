//: Playground - noun: a place where people can play

import Cocoa

func printPersonalGreetings(to names: String...){
    for name in names{
        print("Hello \(name), welcome to Jurassic Park!")
    }
}

printPersonalGreetings(to: "John", "Javid", "Brian", "Eman")

func divisionDescriptionFor(numerator: Double,
                            denominator: Double,
                            withPunctuation punctuation: String = ".") -> String{
    return "\(numerator) divided by \(denominator) equals \(numerator / denominator)\(punctuation)"
}

print(divisionDescriptionFor(numerator: 9.0, denominator: 3.0, withPunctuation: "!"))
print(divisionDescriptionFor(numerator: 9.0, denominator: 3.0))

var error = "The request failed:"
func appendErrorCode(_ code: Int, toErrorString errorString: inout String){
    if code == 400{
        errorString += " bad request."
    }
}

appendErrorCode(400, toErrorString: &error)
print(error)


func areaOfTriangleWith(base: Double, height: Double) -> Double{
    let numerator = base * height
    
    func divide() -> Double{
        return numerator / 2
    }
    
    return divide()
}

print(areaOfTriangleWith(base: 3.0, height: 5.0))

func sortedEvenOddNumbers(_ numbers: [Int]) -> (evens: [Int], odds: [Int]){
    var evens = [Int]()
    var odds = [Int]()
    
    for number in numbers{
        if number % 2 == 0 {
            evens.append(number)
        }
        else{
            odds.append(number)
        }
    }
    
    return (evens, odds)
}

let aBunchOfNumbers = [10, 1, 4, 3, 57, 43, 84, 27, 156, 111]
let theSortedNumbers = sortedEvenOddNumbers(aBunchOfNumbers)
print("evens are: \(theSortedNumbers.evens)\nthe odd numbers are: \(theSortedNumbers.odds)")

func grabMiddleName(fromFullName name: (String, String?, String)) -> String?{
    return name.1
}
let middleName = grabMiddleName(fromFullName: ("Matt", "Matherson", "Mathias"))
if let theName = middleName{
    print(theName)
}

func printMiddleName(fromFullName name: (first: String,
    middle: String?,
    last: String)){
    guard let middleName = name.middle else{
        print("Hey there!")
        return
    }
    print("Hey \(middleName)")
}

printMiddleName(fromFullName: ("Matt", nil, "Mathias"))


//bronze challenge
func printMiddleName2(fromFullName name: (first: String, middle: String?, last: String)){
    
    guard let middleName = name.middle, middleName.characters.count < 4 else{
        print("Hey there!")
        return
    }
    print("Hey \(middleName)")
}

printMiddleName2(fromFullName: ("Matt", "Tom", "Mathias"))


//silver challenge
func siftBeans(fromGroceryList list: [String]) -> (beans: [String], otherGroceries: [String]) {
    
    var beans = [String]()
    var otherGroceries = [String]()
    
    for item in list {
        if item.hasSuffix("beans"){
            beans.append(item)
        }
        else{
            otherGroceries.append(item)
        }
    }
    
    return (beans, otherGroceries)
}

let result = siftBeans(fromGroceryList: ["green beans", "milk", "black beans", "pinto beans", "apples"])

print(result.beans)
print(result.otherGroceries)











