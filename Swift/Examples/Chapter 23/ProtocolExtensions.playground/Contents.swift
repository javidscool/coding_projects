//: Playground - noun: a place where people can play

import Cocoa

protocol Exercise: CustomStringConvertible{
    var name: String { get }
    var caloriesBurned: Double { get }
    var minutes: Double { get }
    
    //bronze challenge
    var title: String { get }
}

struct EllipticalWorkout: Exercise{
    let name = "Elliptical Workout"
    let title = "Go Fast Machine 3000"
    let caloriesBurned: Double
    let minutes: Double
}

struct TreadmillWorkout: Exercise{
    let name = "Treadmill Workout"
    let caloriesBurned: Double
    let minutes: Double
    let laps: Double
}

extension Exercise{
    var caloriesBurnedPerMinute: Double{
        return caloriesBurned / minutes
    }
}

extension Exercise{
    var description: String{
        return "Exercise(\(name), burned \(caloriesBurned) calories in \(minutes) minutes)"
    }
}

extension Exercise{
    var title: String{
        return "\(name) - \(minutes) minutes"
    }
}

extension TreadmillWorkout{
    var description: String{
        return "Treadmill(\(caloriesBurned) calories and \(laps) laps in \(minutes) minutes)"
    }
}

extension Sequence where Iterator.Element == Exercise{
    func totalCaloriesBurned() -> Double{
        var total: Double = 0
        for exercise in self{
            total += exercise.caloriesBurned
        }
        
        return total
    }
}

let ellipticalWorkout = EllipticalWorkout(caloriesBurned: 335, minutes: 30)

let runningWorkout = TreadmillWorkout(caloriesBurned: 350, minutes: 25, laps: 10.5)

print(ellipticalWorkout.caloriesBurnedPerMinute)
print(runningWorkout.caloriesBurnedPerMinute)

let mondayWorkout: [Exercise] = [ellipticalWorkout, runningWorkout]
print(mondayWorkout.totalCaloriesBurned())

print(ellipticalWorkout)
print(runningWorkout)

for exercise in mondayWorkout{
    print(exercise.title)
}

print(ellipticalWorkout.title)
