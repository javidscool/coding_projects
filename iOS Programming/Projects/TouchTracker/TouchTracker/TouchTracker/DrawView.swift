//
//  DrawView.swift
//  TouchTracker
//
//  Created by Javid Shamloo on 3/1/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

//I did the silver and gold challenges for chapter 18
//I did the silver, gold, and platinum challenges for chapter 19

import UIKit

class DrawView : UIView, UIGestureRecognizerDelegate {
    var currentLines = [NSValue:Line]()
    var finishedLines = [Line]()
    
    var currentCircle = Circle()
    var finishedCircles = [Circle]()
    
    var selectedLineIndex: Int?{
        didSet{
            //take out the menu if we no longer have a line 
            //attached to it
            if selectedLineIndex == nil{
                let menu = UIMenuController.shared
                menu.setMenuVisible(false, animated: true)
            }
        }
    }
    
    
    var moveRecognizer: UIPanGestureRecognizer!
    var longPressRecognizer: UILongPressGestureRecognizer!
    
    
    //starting max and min velocity at opposite values so we can update them properly
    var maxRecordedVelocity: CGFloat = CGFloat.leastNonzeroMagnitude
    var minRecordedVelocity: CGFloat = CGFloat.greatestFiniteMagnitude
    
    var currentVelocity: CGFloat = 0
    var currentLineWidth: CGFloat{
        let maxLineWidth: CGFloat = 20
        let minLineWidth: CGFloat = 1

        //this equation will keep us between the max and min line width
        //and will also make it so faster velocity makes for a thinner line
        
        //this bounds our velocity to a weight between 0 and 1
        let boundedVelocity =
            (maxRecordedVelocity - currentVelocity) /
            (maxRecordedVelocity - minRecordedVelocity)
        
        //this binds our line width between 1 and 20, taking the bounded velocity as
        //the weight as to where the final line width ends up
        //remember, faster velocity equals thinner line
        let lineWidth =
            (boundedVelocity * (maxLineWidth - minLineWidth)) + minLineWidth
        
        return lineWidth
    }

    
    @IBInspectable var finishedLineColor: UIColor = UIColor.black{
        didSet{
            setNeedsDisplay()
        }
    }
    
    @IBInspectable var currentLineColor: UIColor = UIColor.red{
        didSet{
            setNeedsDisplay()
        }
    }
    
    @IBInspectable var lineThickness: CGFloat = 10{
        didSet{
            setNeedsDisplay()
        }
    }
    
    func stroke(_ line: Line){
        line.lineColor.setStroke()
        
        let path = UIBezierPath()
        path.lineWidth = line.width
        path.lineCapStyle = .round
        
        path.move(to: line.begin)
        path.addLine(to: line.end)
        path.stroke()
    }
    
    override func draw(_ rect: CGRect) {
        finishedLineColor.setStroke()
        for line in finishedLines{
            //map the line color to the angle
            line.color.setStroke()
            stroke(line)
        }
        
        currentLineColor.setStroke()
        for(_, line) in currentLines {
            //map the line color to the angle
            line.color.setStroke()
            stroke(line)
        }
        
        if let index = selectedLineIndex {
            UIColor.green.setStroke()
            let selectedLine = finishedLines[index]
            stroke(selectedLine)
        }
        
        //lastly, draw the circles
        finishedLineColor.setStroke()
        for circle in finishedCircles{
            let path = UIBezierPath(ovalIn: circle.rect)
            path.lineWidth = lineThickness
            path.stroke()
        }
        currentLineColor.setStroke()
        UIBezierPath(ovalIn: currentCircle.rect).stroke()
    }
    
    func updateCurrentCircle(_ touchesArray: Array<UITouch>){
        //get the locations of the touches
        let location1 = touchesArray[0].location(in: self)
        let location2 = touchesArray[1].location(in: self)
        
        //then create our circle
        currentCircle = Circle(point1: location1, point2: location2)
    }
    
    override func touchesBegan(_ touches: Set<UITouch>,
                               with event: UIEvent?) {
        
        //if we have two touches, then we're drawing a circle
        if touches.count == 2{
            updateCurrentCircle(Array(touches))
        }
        else{
            for touch in touches {
                let location = touch.location(in: self)
                
                let newLine = Line(begin: location, end: location,
                                   width: currentLineWidth, lineColor: currentLineColor)
                
                let key = NSValue(nonretainedObject: touch)
                currentLines[key] = newLine
            }
        }
        setNeedsDisplay()
    }
    
    override func touchesMoved(_ touches: Set<UITouch>,
                               with event: UIEvent?) {
        
        //if we have two touches, we're drawing a circle
        if touches.count == 2{
            updateCurrentCircle(Array(touches))
        }
        
        else{
            for touch in touches{
                let key = NSValue(nonretainedObject: touch)
                currentLines[key]?.end = touch.location(in: self)
                currentLines[key]?.width = currentLineWidth
            }
        }
        
        setNeedsDisplay()
    }
    
    override func touchesEnded(_ touches: Set<UITouch>,
                               with event: UIEvent?) {
        
        //if we have two touches, we're finished drawing a circle
        //we need to do the or statement because the end circle touch detection is fucked up
        if touches.count == 2 || (currentCircle.rect != CGRect.zero){
            
            if touches.count == 2{
                print("updating circle last time")
                updateCurrentCircle(Array(touches))
            }
            finishedCircles.append(currentCircle)
            
            //reset current circle
            currentCircle = Circle()
        }
        
        else{
            
            for touch in touches {
                let key = NSValue(nonretainedObject: touch)
                if var line = currentLines[key] {
                    line.end = touch.location(in: self)
                    
                    line.width = currentLineWidth
                    line.lineColor = currentLineColor
                    finishedLines.append(line)
                    
                    currentLines.removeValue(forKey: key)
                }
            }
        }
        
        setNeedsDisplay()    
    }
    
    override func touchesCancelled(_ touches: Set<UITouch>?,
                                   with event: UIEvent?) {
        
        currentLines.removeAll()
        currentCircle = Circle()
        
        setNeedsDisplay()    
    }
    
    required init?(coder aDecoder: NSCoder){
        super.init(coder: aDecoder)
        
        let doubleTapRecognizer = UITapGestureRecognizer(target: self, action: #selector(DrawView.doubleTap(_:)))
        
        doubleTapRecognizer.numberOfTapsRequired = 2
        doubleTapRecognizer.delaysTouchesBegan = true
        addGestureRecognizer(doubleTapRecognizer)
        
        
        let tapRecognizer =
            UITapGestureRecognizer(target: self,
                                action: #selector(DrawView.tap(_:)))
        tapRecognizer.delaysTouchesBegan = true
        
        tapRecognizer.require(toFail: doubleTapRecognizer)
       	addGestureRecognizer(tapRecognizer)
        
        
        longPressRecognizer = UILongPressGestureRecognizer(
            target: self, action: #selector(DrawView.longPress(_:)))
        addGestureRecognizer(longPressRecognizer)
        
        
        moveRecognizer = UIPanGestureRecognizer(target: self,
                                                action: #selector(DrawView.moveLine(_:)))
        moveRecognizer.delegate = self
        moveRecognizer.cancelsTouchesInView = false
        addGestureRecognizer(moveRecognizer)
        
        
        let swipeRecognizer = UISwipeGestureRecognizer(target: self,
                                                action: #selector(DrawView.swipeForColors(_:)))
        //make it so that we need a three-finger swipe up to trigger this gesture
        swipeRecognizer.numberOfTouchesRequired = 3
        swipeRecognizer.direction = .up
        addGestureRecognizer(swipeRecognizer)
    }
    
    func gestureRecognizer(_ gestureRecognizer: UIGestureRecognizer, shouldRecognizeSimultaneouslyWith otherGestureRecognizer: UIGestureRecognizer) -> Bool {
        return true
    }
    
    func doubleTap(_ gestureRecognizer: UIGestureRecognizer){
        print("Recognized a double tap")
        
        selectedLineIndex = nil
        currentLines.removeAll()
        finishedLines.removeAll()
        
        currentCircle = Circle()
        finishedCircles.removeAll()
        
        setNeedsDisplay()
    }
    
    func tap(_ gestureRecognizer: UIGestureRecognizer){
        print("Recognized a tap")
        
        let point = gestureRecognizer.location(in: self)
        selectedLineIndex = indexOfLine(at: point)
        
        //grab the menu controller
        let menu = UIMenuController.shared
        
        if selectedLineIndex != nil{
            
            //make DrawView the target of menu item action messages
            becomeFirstResponder()
            
            //create a new "Delete" UIMenuItem
            let deleteItem = UIMenuItem(title: "Delete",
                                        action: #selector(DrawView.deleteLine(_:)))
            menu.menuItems = [deleteItem]
            
            //tell the menu where it should come from and show it
            let targetRect = CGRect(x: point.x, y: point.y,
                                    width: 2, height: 2)
            menu.setTargetRect(targetRect, in: self)
            menu.setMenuVisible(true, animated: true)
        }
        else{
            //hide the menu if no line is selected
            menu.setMenuVisible(false, animated: true)
        }
        
        setNeedsDisplay()    
    }
    
    func longPress(_ gestureRecognizer: UIGestureRecognizer){
        print("Recognized a long press")
        
        if gestureRecognizer.state == .began {
            let point = gestureRecognizer.location(in: self)
            selectedLineIndex = indexOfLine(at: point)
            
            if selectedLineIndex != nil {
                currentLines.removeAll()
            }
        }
        else if gestureRecognizer.state == .ended {
            selectedLineIndex = nil
        }
        
        setNeedsDisplay()
    }
    
    func swipeForColors(_ gestureRecognizer: UISwipeGestureRecognizer) {
        print("Bringing up color menu")
        currentLines.removeAll()
        
        //we're gonna use the UIMenuController to change colors
        //which is actually a more efficient option than bringing up a whole panel
        
        //grab the menu controller
        let menu = UIMenuController.shared
        
        //make DrawView the target of menu item action messages
        becomeFirstResponder()
        
        let color0Item = UIMenuItem(title: "Black",
                                    action: #selector(DrawView.selectColor0(_:)))
        let color1Item = UIMenuItem(title: "Gray",
                                    action: #selector(DrawView.selectColor1(_:)))
        let color2Item = UIMenuItem(title: "Red",
                                    action: #selector(DrawView.selectColor2(_:)))
        let color3Item = UIMenuItem(title: "Yellow",
                                    action: #selector(DrawView.selectColor3(_:)))
        let color4Item = UIMenuItem(title: "Blue",
                                    action: #selector(DrawView.selectColor4(_:)))
        
        menu.menuItems = [color0Item, color1Item, color2Item, color3Item, color4Item]
        
        //tell the menu where it should come from and show it
        let targetRect = CGRect(x: self.frame.midX, y: self.frame.midY,
                                width: 2, height: 2)
        menu.setTargetRect(targetRect, in: self)
        menu.setMenuVisible(true, animated: true)
    }
    
    func selectColor0(_ sender: UIMenuController){
        currentLineColor = .black
        UIMenuController.shared.setMenuVisible(false, animated: true)
    }
    
    func selectColor1(_ sender: UIMenuController){
        currentLineColor = .gray
        UIMenuController.shared.setMenuVisible(false, animated: true)
    }
    
    func selectColor2(_ sender: UIMenuController){
        currentLineColor = .red
        UIMenuController.shared.setMenuVisible(false, animated: true)
    }
    
    func selectColor3(_ sender: UIMenuController){
        currentLineColor = .yellow
        UIMenuController.shared.setMenuVisible(false, animated: true)
    }
    
    func selectColor4(_ sender: UIMenuController){
        currentLineColor = .blue
        UIMenuController.shared.setMenuVisible(false, animated: true)
    }
    
    func indexOfLine(at point: CGPoint) -> Int?{
        //find a line close to point
        for (index, line) in finishedLines.enumerated(){
            let begin = line.begin
            let end = line.end
            
            //check a few points on the line
            for t in stride(from: CGFloat(0), to: 1.0, by: 0.05){
                let x = begin.x + ((end.x - begin.x) * t)
                let y = begin.y + ((end.y - begin.y) * t)
                
                //if the tapped point is within 20 points, let's return this line
                //index
                if hypot(x - point.x, y - point.y) < 20.0{
                    return index
                }
            }
        }
        
        //if nothing is close enough to the tapped point, then we did not
        //select a line
        return nil
    }
    
    override var canBecomeFirstResponder: Bool {
        return true
    }
    
    func deleteLine(_ sender: UIMenuController){
        //remove the selected line from the list of finishedLines
        if let index = selectedLineIndex{
            finishedLines.remove(at: index)
            selectedLineIndex = nil
            
            //redraw everything
            setNeedsDisplay()
        }
    }
    
    func moveLine(_ gestureRecognizer: UIPanGestureRecognizer){
        print("Recognized a pan")
        
        //get the velocity from the gesture recognizer
        let velocityInXY = gestureRecognizer.velocity(in: self)
        currentVelocity = hypot(velocityInXY.x, velocityInXY.y)
        
        //record max and min velocities to use to modulate velocity of line
        maxRecordedVelocity = max(maxRecordedVelocity, currentVelocity)
        minRecordedVelocity = min(minRecordedVelocity, currentVelocity)
        
        print("Current Drawing Velocity: \(currentVelocity) points per second")
        print("maxRecordedVelocity: \(maxRecordedVelocity) points per second")
        print("minRecordedVelocity: \(minRecordedVelocity) points per second")
        
        
        //we only move lines on long presses
        guard longPressRecognizer.state == .changed || longPressRecognizer.state == .ended else{
            return
        }
        
        //if a line is selected
        if let index = selectedLineIndex{
            //when the pan recognizer changes its position...
            if gestureRecognizer.state == .changed{
                //how far has the pan moved?
                let translation = gestureRecognizer.translation(
                    in: self)
                
                //add the translation to the current beginning and end points
                //of the line
                finishedLines[index].begin.x += translation.x
                finishedLines[index].begin.y += translation.y
                finishedLines[index].end.x += translation.x
                finishedLines[index].end.y += translation.y
                
                //resetting the translation point
                gestureRecognizer.setTranslation(CGPoint.zero, in: self)
                
                //redraw the screen
                setNeedsDisplay()
            }
        }
        else{
            //if no line is selected, don't do anything
            return
        }
    }
}
