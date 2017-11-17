//: Playground - noun: a place where people can play

import Cocoa

let volunteerCounts = [1,3,40,32,2,53,77,13]

let volunteersSorted = volunteerCounts.sorted{ $0 < $1 }
print(volunteersSorted)

func makeTownGrand() -> (Int, Int) -> Int {
    func buildRoads(byAddingLights lights: Int, toExistingLights existingLights: Int) -> Int {
        return lights + existingLights
    }
    
    return buildRoads
}

var stopLights = 4
let townPlanAddsLights = makeTownGrand()

stopLights = townPlanAddsLights(4, stopLights)
print(stopLights)

func makeTownReallyGrand(withBudget budget: Int, condition: (Int) -> Bool) -> ((Int, Int) -> Int)?{
    if condition(budget){
        func buildRoads(byAddingLights lights: Int, toExistingLights existingLights: Int) -> Int{
            return lights + existingLights
        }
        
        return buildRoads
    }
    else{
        return nil
    }
}

func evaluate(budget: Int) -> Bool {
    return budget > 10_000
}

stopLights = 4

if let townAddsLights = makeTownReallyGrand(withBudget: 100_000, condition: evaluate){
    stopLights = townAddsLights(6, stopLights)
}
print("we have \(stopLights) stoplights.")

func makePopulationTracker(forInitialPopulation population: Int) -> ((Int) -> Int) {
    var totalPopulation = population
    
    func populationTracker(growth: Int) -> Int{
        totalPopulation += growth
        return totalPopulation
    }
    
    return populationTracker
}

var currentPopulation = 5_422
let growBy = makePopulationTracker(forInitialPopulation: currentPopulation)

growBy(500)
growBy(500)
growBy(500)
growBy(500)
currentPopulation = growBy(1000)
print(currentPopulation)

let anotherGrowBy = growBy
print(anotherGrowBy(500))

let precinctPopulations = [1244, 2021, 2157]
let projectedPopulations = precinctPopulations.map {
    (population: Int) -> Int in
        return population * 2
}
print(projectedPopulations)

let bigProjections = projectedPopulations.filter {
    (projection: Int) -> Bool in
        return projection > 4000
}
print(bigProjections)

let totalProjection = projectedPopulations.reduce(0){
    (accumulatedProjection: Int, precinctProjection: Int) -> Int in
        return accumulatedProjection + precinctProjection
}
print(totalProjection)


//bronze challenge 1
var volunteerCounts2 = [1,3,40,32,2,53,77,13]

volunteerCounts2.sort{ $0 < $1 }
print(volunteerCounts2)


//bronze challenge 2
var volunteerCounts3 = [1,3,40,32,2,53,77,13]
volunteerCounts3.sort()
print(volunteerCounts3)


//gold challenge
let precinctPopulations2 = [1244, 2021, 2157]
let projectedPopulations2 = precinctPopulations.map { $0 * 2 }
print(projectedPopulations2)

let bigProjections2 = projectedPopulations2.filter { $0 > 4000 }
print(bigProjections2)

let totalProjection2 = projectedPopulations2.reduce(0){ $0 + $1 }
print(totalProjection2)

