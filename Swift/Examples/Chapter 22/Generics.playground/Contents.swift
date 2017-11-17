//: Playground - noun: a place where people can play

import Cocoa

struct StackIterator<T>: IteratorProtocol{
    //typealias Element = T
    
    var stack: Stack<T>
    
    mutating func next() -> /*Element*/ T? {
        return stack.pop()
    }
}

struct Stack<Element>: Sequence{
    var items = [Element]()
    
    //bronze challenge
    mutating func filter(_ f: (Element) -> Bool) -> Stack<Element> {
        
        var filteredItems = Stack<Element>()
        
        for item in items{
            if f(item){
                filteredItems.push(item)
            }
        }
        
        return filteredItems
    }
    
    mutating func push(_ newItem: Element){
        items.append(newItem)
    }
    
    mutating func pop() -> Element?{
        guard !items.isEmpty else{
            return nil
        }
        
        return items.removeLast()
    }
    
    func map<U>(_ f: (Element) -> U) -> Stack<U>{
        var mappedItems = [U]()
        for item in items{
            mappedItems.append(f(item))
        }
        return Stack<U>(items: mappedItems)
    }
    
    func makeIterator() -> StackIterator<Element> {
        return StackIterator(stack: self)
    }
    
    mutating func pushAll<S: Sequence>(_ sequence: S) where S.Iterator.Element == Element {
        for item in sequence {
            self.push(item)
        }
    }
}

var intStack = Stack<Int>()
intStack.push(1)
intStack.push(2)
var doubledStack = intStack.map({ 2 * $0 })

print(intStack.pop())
print(intStack.pop())
print(intStack.pop())

print(doubledStack.pop())
print(doubledStack.pop())

var stringStack = Stack<String>()
stringStack.push("This is a string")
stringStack.push("another string")

print(stringStack.pop())

func myMap<T, U>(_ items: [T], _ f: (T) -> (U)) -> [U]{
    var result = [U]()
    
    for item in items{
        result.append(f(item))
    }
    return result
}

let strings = ["one", "two", "three"]
let stringLengths = myMap(strings){ $0.characters.count }
print(stringLengths)

func checkIfEqual<T: Equatable>(_ first: T, _ second: T) -> Bool{
    return first == second
}
print(checkIfEqual(1, 1))
print(checkIfEqual("a string", "a string"))
print(checkIfEqual("a string", "a strung"))

func checkIfDescriptionsMatch<T: CustomStringConvertible, U: CustomStringConvertible>(_ first: T, _ second: U) -> Bool{
    return first.description == second.description
}

print(checkIfDescriptionsMatch(Int(1), UInt(1)))
print(checkIfDescriptionsMatch(1, 1.0))
print(checkIfDescriptionsMatch(Float(1.0), Double(1.0)))

var myStack = Stack<Int>()
myStack.push(10)
myStack.push(20)
myStack.push(30)

var myStackGenerator = StackIterator(stack: myStack)
while let value = myStackGenerator.next(){
    print("got \(value)")
}

for value in myStack{
    print("for-in loop: got \(value)")
}

var myOtherStack = Stack<Int>()
myOtherStack.pushAll([1, 2, 3])
myStack.pushAll(myOtherStack)
for value in myStack{
    print("after pushing: got \(value)")
}


//bronze challenge
var testStack = Stack<Int>()
testStack.push(10)
testStack.push(20)
testStack.push(30)
testStack.push(50)
testStack.push(100)
testStack.push(150)
testStack.push(200)
testStack.push(250)

var filteredStack = testStack.filter{ $0 >= 200 }
for value in filteredStack{
    print("for-in loop: got \(value)")
}


//silver challenge
func findAll<T: Equatable>(_ array: [T], _ element: T) -> [Int]?{
    
    var foundIndexes = [Int]()
    
    for i in 0..<array.count{
        if array[i] == element{
            foundIndexes.append(i)
        }
    }
    
    if foundIndexes.isEmpty{
        return nil
    }
    
    return foundIndexes
}

var newArray = findAll([5, 3, 7, 3, 9], 8)
print(newArray ?? "nothing")
var newStrLoc = findAll(Array("hello".characters), "z")
print(newStrLoc ?? "nada")


//gold challenge
func findAll2<T: Equatable, C: Collection>(_ array: C, _ element: T)
    -> [C.Index]? where C.Iterator.Element == T{
    
    var foundIndexes = Array<C.Index>()
    var index = array.startIndex
    
    for item in array{
        if item == element{
            foundIndexes.append(index)
        }
        
        //increment our index position
        index = array.index(after: index)
    }
    
    if foundIndexes.isEmpty{
        return nil
    }
        
    return foundIndexes
}

var newArray2 = findAll2([5, 3, 7, 3, 9], 3)
print(newArray2 ?? "nothing")
var newStrLoc2 = findAll2(Array("hello".characters), "l")
print(newStrLoc2 ?? "nada")