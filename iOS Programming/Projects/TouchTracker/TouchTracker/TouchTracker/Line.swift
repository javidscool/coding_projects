//
//  Line.swift
//  TouchTracker
//
//  Created by Javid Shamloo on 3/1/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

//I did the silver and gold challenges for chapter 18
//I did the silver, gold, and platinum challenges for chapter 19

import UIKit
import CoreGraphics

struct Line{
    var begin = CGPoint.zero
    var end = CGPoint.zero
    
    var width: CGFloat = 10
    var lineColor: UIColor = .black
}

extension Line{
    
    var angle: Measurement<UnitAngle>{
        //if begin and end are the same, we have 0 rad angle
        guard begin != end else{
            return Measurement(value: 0.0, unit: .radians)
        }
        
        //else we use difY/difX = tan(angle)
        let dy = Double(end.y - begin.y)
        let dx = Double(end.x - begin.x)
        
        let angleInRadians: Measurement<UnitAngle> =
            Measurement(value: atan2(dy, dx), unit: .radians)
        
        return angleInRadians
    }
    
    var color: UIColor{
        //first we map the angle in radians to 0 to 2pi
        let ratio = (self.angle.value + Double.pi) / (Double.pi * 2)
        
        //convert to degrees
        let angle = ratio * 180 / Double.pi
        
        let hueVal = angle / 360
        
        return UIColor(hue: CGFloat(hueVal), saturation: 1, brightness: 1, alpha: 0.6)
    }
}
