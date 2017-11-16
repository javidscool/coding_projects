//
//  Mayor.swift
//  MonsterTown
//
//  Created by Javid Shamloo on 3/14/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import Foundation

//silver challenge
struct Mayor{
    
    //gold challenge
    private var anxietyLevel: Int = 0
    
    mutating func makeStatement(attacked attackMade: Bool){
        
        anxietyLevel = anxietyLevel + 1
        
        if attackMade {
            print("I'm deeply saddened to hear about this latest tragedy.  I promise that my office is looking into the nature of this rash of violence.")
        }
    }
    
    func printAnxiety(){
        print("Mayor anxiety: \(anxietyLevel)")
    }
}
