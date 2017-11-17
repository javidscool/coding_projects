//: Playground - noun: a place where people can play

import Cocoa

typealias Velocity = Double

extension Velocity{
    var kph: Velocity { return self * 1.60934 }
    var mph: Velocity { return self }
}

protocol VehicleType{
    var topSpeed: Velocity { get }
    var numberOfDoors: Int { get }
    var hasFlatbed: Bool { get }
}

struct Car{
    let make: String
    let model: String
    let year: Int
    let numberOfDoors: Int //added for bronze challenge 2
    let color: String
    let nickname: String
    var gasLevel: Double{
        willSet{
            precondition(newValue <= 1.0 && newValue >= 0.0, "New Value must be between 0 and 1")
        }
    }
}

extension Car: VehicleType{
    var topSpeed: Velocity { return 180 }
    var hasFlatbed: Bool { return false }
}

extension Car{
    init(carMake: String, carModel: String, carYear: Int){
        self.init(make: carMake,
                  model: carModel,
                  year: carYear,
                  numberOfDoors: 4,
                  color: "Black",
                  nickname: "N/A",
                  gasLevel: 1.0)
    }
}

extension Car{
    enum CarKind: CustomStringConvertible{
        case coupe, sedan
        var description: String{
            switch self{
            case .coupe:
                return "Coupe"
            case .sedan:
                return "Sedan"
            }
        }
    }
    
    var kind: CarKind{
        if numberOfDoors == 2{
            return .coupe
        }
        else{
            return .sedan
        }
    }
}

extension Car{
    mutating func emptyGas(amount: Double){
        precondition(amount <= 1 && amount > 0, "Amount to remove must be between 0 and 1.")
        
        //silver challenge
        if amount > gasLevel{
            gasLevel = 0
        }
        else{
            gasLevel -= amount
        }
    }
    
    mutating func fillGas(){
        gasLevel = 1.0
    }
}

var c = Car(carMake: "Ford", carModel: "Fusion", carYear: 2013)
c.kind.description
c.emptyGas(amount: 0.3)
c.gasLevel
c.fillGas()
c.gasLevel
var myCar: Velocity = 1.234

myCar.kph


//bronze challenge 1
extension Int{
    var timesFive: Int {
        return self * 5
    }
}

var num: Int = 5
print(num.timesFive)


//bronze challenge 2
var car2 = Car(make: "Lexus", model: "LS430", year: 2004, numberOfDoors: 2, color: "Silver", nickname: "coolguy", gasLevel: 1.0)
print(car2.kind)


//silver challenge
car2.emptyGas(amount: 0.7)
car2.emptyGas(amount: 0.7)
print(car2.gasLevel)
