//
//  Circle.swift
//  TouchTracker
//
//  Created by Javid Shamloo on 3/24/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import CoreGraphics

struct Circle{
    var rect = CGRect.zero
    
    init(){
        self.rect = CGRect.zero
    }
    
    init(rect: CGRect){
        self.rect = rect
    }
    
    //circle is triggered by two touches
    //calculating center of circle by the midpoint of the two touches
    init(point1: CGPoint, point2: CGPoint){
        
        //ok, so what we're trying to do is create a circle to fit within this rectangle
        //in order to do that, we first need to figure out which of the two dimensions of the 
        //rectangle are smaller, because that's the maximum circle we can fit inside of it
        
        let widthOfRect = abs(point2.x - point1.x)
        let heightOfRect = abs(point2.y - point1.y)
        
        let diameter = min(widthOfRect, heightOfRect)
        
        
        //now that we have the diameter, get its radius
        let radius = diameter / 2
        
        //use the mid-point formula to calculate the center of the two touches
        let center = CGPoint(x: (point1.x + point2.x) / 2, y: (point1.y + point2.y) / 2)
        
        self.rect = CGRect(x: (center.x - radius), y: (center.y - radius),
                           width: diameter, height: diameter)
    }
}
