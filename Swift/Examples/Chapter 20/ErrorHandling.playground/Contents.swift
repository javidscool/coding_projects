//: Playground - noun: a place where people can play

import Cocoa

//I SOLVED THE BRONZE, SILVER, AND GOLD CHALLENGES

enum Token{
    case number(Int)
    case plus
    case minus
    case multiplication
    case division
}

class Lexer{
    let input: String.CharacterView
    var position: String.CharacterView.Index
    
    enum Error: Swift.Error{
        case InvalidCharacter(character: Character, position: String.CharacterView.Index)
    }
    
    init(input: String){
        self.input = input.characters
        self.position = self.input.startIndex
    }
    
    func peek() -> Character?{
        guard position < input.endIndex else{
            return nil
        }
        return input[position]
    }
    
    func advance(){
        assert(position < input.endIndex, "Cannot advance past the end!")
        position = input.index(after: position)
    }
    
    func getNumber() -> Int{
        var value = 0
        
        while let nextCharacter = peek(){
            switch nextCharacter{
            case "0"..."9":
                //another digit - add it into value
                let digitValue = Int(String(nextCharacter))!
                value = 10 * value + digitValue
                advance()
                
            default:
                //a non-digit - go back to regular lexing
                return value
            }
        }
        
        return value
    }
    
    //return the token and the index of the beginning of each token
    //so, something like 100 will have the index of the number "1" recorded and returned
    func lex() throws -> ([Token], [String.CharacterView.Index]){
        
        var tokens = [Token]()
        var tokenIndex: [String.CharacterView.Index] = []
        
        while let nextCharacter = peek(){
            switch nextCharacter{
            case "0" ... "9":
                //get index of this token
                tokenIndex.append(position)
                
                //start of a number - need to grab the rest
                let value = getNumber()
                tokens.append(.number(value))
            case "+":
                //get index of this token
                tokenIndex.append(position)
                
                tokens.append(.plus)
                advance()
            case "-":
                //get index of this token
                tokenIndex.append(position)
                
                tokens.append(.minus)
                advance()
            case "*":
                //get index of this token
                tokenIndex.append(position)
                
                tokens.append(.multiplication)
                advance()
            case "/":
                //get index of this token
                tokenIndex.append(position)
                
                tokens.append(.division)
                advance()
            case " ":
                //just advance to ignore spaces
                advance()
            default:
                //something unexpected - need to send back an error
                throw Error.InvalidCharacter(character: nextCharacter, position: position)
            }
        }
        
        return (tokens, tokenIndex)
    }
}

class Parser{
    enum Error: Swift.Error{
        case UnexpectedEndOfInput
        case InvalidToken(token: Token, position: String.CharacterView.Index)
    }
    
    let tokens: [Token]
    var tokenIndex: [String.CharacterView.Index]
    var position = 0
    var currentIndex = 0
    
    init(tokenResult: (tokenArray: [Token], tokenIndexArray: [String.CharacterView.Index])){
        self.tokens = tokenResult.tokenArray
        self.tokenIndex = tokenResult.tokenIndexArray
    }
    
    func getNextToken() -> Token?{
        guard position < tokens.count else{
            return nil
        }
        
        let token = tokens[position]
        
        //get our current index position before we increment
        currentIndex = position
        
        position += 1
        return token
    }
    
    func getNumber() throws -> Int{
        guard let token = getNextToken() else{
            throw Error.UnexpectedEndOfInput
        }
        
        switch token{
        case .number(let value):
            return value
        case .plus, .minus, .multiplication, .division:
            throw Error.InvalidToken(token: token, position: tokenIndex[currentIndex])
        }
    }
    
    func parse() throws -> Int{
        //require a number first
        var value = try getNumber()
        
        while let token = getNextToken(){
            switch token{
            //getting plus, minus, mult, and division after a number is legal
                
            //ok, so what we're doing here is recursively calling parse() again in the plus and minus
            //cases to take care of the groups of multiplication/division first
            //we drill down until we have numbers and mult/division, which we then execute
            //the result from these higher precedence operations is then bubbled up to be added/subtracted
            case .plus:
                let nextNumber = try parse()
                value += nextNumber
            case .minus:
                let nextNumber = try parse()
                value -= nextNumber
            case .multiplication:
                let nextNumber = try getNumber()
                value *= nextNumber
            case .division:
                let nextNumber = try getNumber()
                value /= nextNumber
            //getting a number after a number is NOT legal
            case .number:
                throw Error.InvalidToken(token: token, position: tokenIndex[currentIndex])
            }
        }
        
        return value
    }
}

func evaluate(input: String){
    print("Evaluating: \(input)")
    let lexer = Lexer(input: input)
    
    do{
        let tokenResult = try lexer.lex()
        print("Lexer output: \(tokenResult.0)")
        
        let parser = Parser(tokenResult: tokenResult)
        let result = try parser.parse()
        print("Parser output: \(result)")
    }
    catch Lexer.Error.InvalidCharacter(let character){
        let distanceToPosition = input.distance(from: lexer.input.startIndex, to: character.position)
        print("Input contained an invalid character at index \(distanceToPosition): \(character.character)")
    }
    catch Parser.Error.UnexpectedEndOfInput{
        print("Unexpected end of input during parsing")
    }
    catch Parser.Error.InvalidToken(let tokenError){
        let distanceToPosition = input.distance(from: lexer.input.startIndex, to: tokenError.position)
        
        let errorToken: String
        
        switch tokenError.token{
        case .number(let value):
            errorToken = "\(value)"
        case.plus:
            errorToken = "+"
        case .minus:
            errorToken = "-"
        case .multiplication:
            errorToken = "*"
        case.division:
            errorToken = "/"
        }
        print("Invalid token during parsing at index \(distanceToPosition): \(errorToken)")
    }
    catch{
        print("An error occurred: \(error)")
    }
}

evaluate(input: "10 + 89")
evaluate(input: "89 - 99")
evaluate(input: "1 + 3 + 7a + 8")
evaluate(input: "10 + 3 3 + 7")

evaluate(input: "10 * 3 + 5 * 3")
evaluate(input: "10 + 3 * 5 + 3")
evaluate(input: "10 + 3 * 5 * 3")
evaluate(input: "10 + 3 * * 5 * 3")
