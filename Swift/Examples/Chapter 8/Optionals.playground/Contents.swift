//: Playground - noun: a place where people can play

import Cocoa

var errorCodeString: String?
var errorDescription: String?
errorCodeString = "404"
if let theError = errorCodeString, let errorCodeInteger = Int(theError), errorCodeInteger == 404 {
    errorDescription = "\(errorCodeInteger + 200): resource was not found."
}

var upperCaseErrorDescription: String? = errorDescription?.uppercased()
print(upperCaseErrorDescription)

upperCaseErrorDescription?.append(" PLEASE TRY AGAIN!")
print(upperCaseErrorDescription)


//errorDescription = nil
let description = errorDescription ?? "No error"
print(description)