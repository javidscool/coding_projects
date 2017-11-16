//
//  TabGraphics.swift
//  Blim-App
//
//  Created by Javid Shamloo on 7/11/16.
//  Copyright © 2016 Televisa. All rights reserved.
//

import UIKit

class TabGraphics{
    
    enum TabTypes: Int{
        case Deselected = 0
        case Highlighted = 1
    }
    
    static let singletonInstance = TabGraphics()
    
    var allGraphics = [UIImage]()
    
    func scaleImage(image: UIImage, newSize: CGSize) -> UIImage{
        UIGraphicsBeginImageContextWithOptions(newSize, false, 0.0)
        
        image.drawInRect(CGRectMake(0, 0, newSize.width, newSize.height))
        let newImage: UIImage = UIGraphicsGetImageFromCurrentImageContext()
        
        UIGraphicsEndImageContext()
        
        return newImage
    }
    
    func initTabGraphics(){
        if var tabDeselected: UIImage = UIImage(named: "TabDeselected"),
            var tabHighlighted: UIImage = UIImage(named: "TabHighlighted"){
            
            //scale the images
            tabDeselected = scaleImage(tabDeselected, newSize: CGSizeMake(30, 30))
            tabHighlighted = scaleImage(tabHighlighted, newSize: CGSizeMake(30, 30))
            
            allGraphics.append(tabDeselected)
            allGraphics.append(tabHighlighted)
        }
    }
}
