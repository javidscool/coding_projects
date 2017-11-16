//: Playground - noun: a place where people can play

import Cocoa

var movieRatings = ["a": 4, "b": 5, "c": 10]
print("I have rated \(movieRatings.count) movies")
let aRating = movieRatings["a"]
movieRatings["b"] = 15
print(movieRatings)

let oldRating: Int? = movieRatings.updateValue(69, forKey: "c")
if let lastRating = oldRating, let currentRating = movieRatings["c"] {
    print("old rating: \(lastRating); current rating: \(currentRating)")
}

movieRatings.removeValue(forKey: "a")
print(movieRatings)

for(key, value) in movieRatings {
    print("The movie \(key) was rated \(value)")
}

for movie in movieRatings.keys {
    print("User has rated \(movie)")
}

let watchedMovies = Array(movieRatings.keys)
let ratings = Array(movieRatings.values)

print(watchedMovies)
print(ratings)


//silver challenge
let floridaZipCodes = ["dade": [33186, 33187, 33188, 33189, 33190],
                       "broward": [33331, 33332, 33333, 33334, 33335],
                       "palm beach": [11111, 11112, 11113, 11114, 11115]]
print("Florida has the following zip codes:")
for zips in floridaZipCodes.values{
    print(zips)
}


//gold challenge
var rowCount: Int = 0
print("Florida has the following zip codes: [", terminator: "")
for zips in floridaZipCodes.values{
    
    if rowCount > 0 && rowCount < floridaZipCodes.count{
        print()
        print("\t\t\t\t\t\t", terminator: "   ")
    }
    
    for zip in zips{
        //checking if we're on the last row && last element
        if rowCount == (floridaZipCodes.count - 1) && (zip == zips.last!){
            print(zip, terminator: "")
        }
        else{
            print(zip, terminator: ", ")
        }
    }
    rowCount = rowCount + 1
}
print("]")


