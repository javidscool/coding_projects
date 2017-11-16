//: Playground - noun: a place where people can play

import Cocoa

var statusCode: Int = 518
var errorString: String = "The request failed with the error:"

/*
//regular switch statement
switch statusCode{
case 400:
    errorString = "Bad request"
    
case 401:
    errorString = "Unauthorized"
    
case 403:
    errorString = "Forbidden"
    
case 404:
    errorString = "Not found"
    
default:
    errorString = "None"
    
}
*/

/*
//switch statement with multiple values
switch statusCode{
case 400, 401, 403, 404:
    errorString = "There was something wrong with the request."
    fallthrough
default:
    errorString += " Please review the request and try again."
    
}
*/

//switch statement with ranges and value binding
switch statusCode{
case 100, 101:
    errorString += " Informational, \(statusCode)."
case 204:
    errorString += " Successful but no content, 204."
case 300...307:
    errorString += " Redirection, \(statusCode)."
case 400...417:
    errorString += " Client error, \(statusCode)."
case 500...505:
    errorString += " Server error, \(statusCode)."
case let unknownCode where (unknownCode >= 200 && unknownCode < 300) || unknownCode > 505:
    errorString = "\(unknownCode) is not a known error code."
default:
    errorString = "Unexpected error encountered."
}

print(errorString)

let error = (code: statusCode, error: errorString)
print(error.error)

let firstErrorCode = 104
let secondErrorCode = 204
let errorCodes = (firstErrorCode, secondErrorCode)

switch errorCodes{
case (404, 404):
    print("No items found.")
case (404, _):
    print("First item not found.")
case (_, 404):
    print("Second item not found.")
default:
    print("All items found.")
}

let age = 40
//silver challenge
if case 18...45 = age, age >= 21, !(age >= 30 && age <= 39){
    print("Cool demographic and of drinking age")
}


//bronze challenge
let point = (x: 1, y: 4)

switch point{
case let q1 where (point.x > 0) && (point.y > 0):
    print("\(q1) is in quadrant 1")
default:
    print("case not covered.")
}
