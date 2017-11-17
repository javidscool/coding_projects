//: Playground - noun: a place where people can play

import Cocoa

protocol TabularDataSource{
    var numberOfRows: Int {get}
    var numberOfColumns: Int {get}
    
    func label(forRow row: Int) -> String
    func label(forColumn column: Int) -> String
    
    func itemFor(row: Int, column: Int) -> String
}

func padding(amount: Int) -> String{
    var paddingString = ""
    for _ in 0 ..< amount {
        paddingString += " "
    }
    return paddingString
}

//silver Challenge
func printTable(_ dataSource: TabularDataSource & CustomStringConvertible){
    print("Table: \(dataSource.description)")
    
    //create arrays of the row and column labels
    let rowLabels = (0 ..< dataSource.numberOfRows).map{ dataSource.label(forRow: $0) }
    let columnLabels = (0 ..< dataSource.numberOfColumns).map{ dataSource.label(forColumn: $0) }
    
    //create an array of the width of each row label
    let rowLabelWidths = rowLabels.map{$0.characters.count}
    
    //determine length of longest row label
    guard let maxRowLabelWidth = rowLabelWidths.max() else{
        return
    }
    
    //also keep track of the width of each column
    var columnWidths = [Int]()
    var largestColWidths = [Int]()
    
    //FIRST ITERATION: Finding largest column paddings
    for columnLabel in columnLabels{
        let columnHeader = " \(columnLabel) |"
        columnWidths.append(columnHeader.characters.count)
    }
    largestColWidths = columnWidths
    
    for i in 0 ..< dataSource.numberOfRows{
        
        for j in 0 ..< dataSource.numberOfColumns{
            
            let item = dataSource.itemFor(row: i, column: j)
            let itemString = " \(item) |"
            
            //figuring out if the column header or item under column header is bigger
            let colDifference = largestColWidths[j] - itemString.characters.count
            if colDifference < 0{
                largestColWidths[j] = itemString.characters.count
            }
            
        }
        
    }
    
    //SECOND ITERATION: Printing things out properly and aligned
    //create first row containing column headers
    var firstRow: String = padding(amount: maxRowLabelWidth) + " |"
    
    for i in 0 ..< columnLabels.count{
        let columnHeader = " \(columnLabels[i]) |"
        
        let paddingAmount = largestColWidths[i] - columnHeader.characters.count
        
        firstRow += padding(amount: paddingAmount) + columnHeader
    }
    print(firstRow)
    
    //now create rest of table
    for i in 0 ..< dataSource.numberOfRows{
        //pad the row label out so they are all the same length
        let paddingAmount = maxRowLabelWidth - rowLabelWidths[i]
        var out = rowLabels[i] + padding(amount: paddingAmount) + " |"
        
        //append each item in this row to our string
        for j in 0 ..< dataSource.numberOfColumns{
            
            let item = dataSource.itemFor(row: i, column: j)
            let itemString = " \(item) |"
            
            //figuring out the padding we need for the result
            let paddingAmount = largestColWidths[j] - itemString.characters.count
            
            //adding the padding before we add the item to print
            out += padding(amount: paddingAmount) + itemString
        }
        
        //done - print it
        print(out)
    }
    
}

struct Person{
    let name: String
    let age: Int
    let yearsOfExperiece: Int
}

struct Department: TabularDataSource, CustomStringConvertible{
    let name: String
    var people = [Person]()
    
    var description: String{
        return "Department: (\(name))"
    }
    
    init(name: String){
        self.name = name
    }
    
    mutating func add(_ person: Person){
        people.append(person)
    }
    
    var numberOfRows: Int{
        return people.count
    }
    
    var numberOfColumns: Int{
        return 3
    }
    
    func label(forRow row: Int) -> String {
        return people[row].name
    }
    
    func label(forColumn column: Int) -> String {
        switch column{
        case 0:
            return "Employee Name"
        case 1:
            return "Age"
        case 2:
            return "Years of Experience"
        default:
            fatalError("Invalid column!")
        }
    }
    
    func itemFor(row: Int, column: Int) -> String {
        let person = people[row]
        switch column{
        case 0:
            return person.name
        case 1:
            return String(person.age)
        case 2:
            return String(person.yearsOfExperiece)
        default:
            fatalError("Invalid column!")
        }
    }
}

struct Book{
    let title: String
    let author: String
    let averageReview: Double
}

//gold Challenge
struct BookCollection: TabularDataSource, CustomStringConvertible{
    let name: String
    var books = [Book]()
    
    var description: String{
        return "Collection name: (\(name))"
    }
    
    init(name: String){
        self.name = name
    }
    
    mutating func add(_ book: Book){
        books.append(book)
    }
    
    var numberOfRows: Int{
        return books.count
    }
    
    var numberOfColumns: Int{
        return 2
    }
    
    func label(forRow row: Int) -> String {
        return books[row].title
    }
    
    func label(forColumn column: Int) -> String {
        switch column{
        case 0:
            return "Author"
        case 1:
            return "Average Review"
        default:
            fatalError("Invalid column!")
        }
    }
    
    func itemFor(row: Int, column: Int) -> String {
        let book = books[row]
        switch column{
        case 0:
            return book.author
        case 1:
            return String(book.averageReview)
        default:
            fatalError("Invalid column!")
        }
    }
}

var department = Department(name: "Engineering")
department.add(Person(name: "Joe", age: 30, yearsOfExperiece: 600000))
department.add(Person(name: "Karen", age: 4000000, yearsOfExperiece: 180))
department.add(Person(name: "Fred", age: 50, yearsOfExperiece: 20))

printTable(department)
//print(department)


var bookCollection = BookCollection(name: "Javid's book collection")
bookCollection.add(Book(title: "Learned Optimism", author: "Martin Seligman", averageReview: 4.3))
bookCollection.add(Book(title: "The War of Art", author: "Steven Pressfield", averageReview: 4.8))
bookCollection.add(Book(title: "8 Pillars of Prosperity", author: "James Allen", averageReview: 0.0006))

printTable(bookCollection)

