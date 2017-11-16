//
//  ConversionViewController.swift
//  WorldTrotter
//
//  Created by Javid Shamloo on 2/23/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import UIKit

//I finished the bronze challenge from chapter 4
//I finished the silver challenge from chapter 5
//I finished the bronze, silver, and gold challenges from chapter 6
//I finished the bronze challenge from chapter 7

class ConversionViewController : UIViewController, UITextFieldDelegate {
    
    @IBOutlet var celciusLabel: UILabel!
    @IBOutlet var textField: UITextField!
    
    //storing the last acceptable user input
    var textBackup = String()
    
    var fahrenheitValue: Measurement<UnitTemperature>? {
        didSet {
            updateCelsiusLabel()
        }
    }
    
    var celsiusValue: Measurement<UnitTemperature>? {
        if let fahrenheitValue = fahrenheitValue {
            return fahrenheitValue.converted(to: .celsius)
        }
        else{
            return nil
        }
    }
    
    @IBAction func farenheitFieldEditingChanged(_ textField: UITextField){
        
        if var text = textField.text{
            
            //we need to check if we entered valid text or not
            //we allow input like 98.6 and -.001, but reject 
            //all other alphanumeric characters
            
            //first, we get rid of the whitespaces
            text = text.trimmingCharacters(in: .whitespaces)
            
            //if we have a number, set our fahrenheit value
            if let number = numberFormatter.number(from: text){
            
                fahrenheitValue = Measurement(
                        value: number.doubleValue,
                        unit: .fahrenheit)
            }
            else{
                
                //get the decimal separator for our current locale
                let currentLocale = Locale.current
                let decimalSeparator = currentLocale.decimalSeparator ?? "."
                
                //check if we have valid input that's NOT YET a number
                if text.isEmpty || text == decimalSeparator || text == "-"
                    || text == "-\(decimalSeparator)"{
                    //we accept the text, but don't set the number yet because we don't have 
                    //enough info to do that yet
                    fahrenheitValue = nil
                }
                else{
                    //reject the text and restore the backup
                    text = textBackup
                }
            }
            
            //replace the text with what we cleaned up the text to be
            textField.text = text
            
            //also set our backup text
            textBackup = text
        }
        else{
            fahrenheitValue = nil
        }
    }

    @IBAction func dismissKeyboard(_ sender: UITapGestureRecognizer){
        textField.resignFirstResponder()
    }
    
    func updateCelsiusLabel(){
        if let celsiusValue = celsiusValue {
            celciusLabel.text = numberFormatter.string(from: NSNumber(value: celsiusValue.value))
        }
        else{
            celciusLabel.text = "???"
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        updateCelsiusLabel()
    }
    
    override func viewWillAppear(_ animated: Bool) {
        
        //get the hour that it currently is
        let date = Date()
        let hour = Calendar.current.component(.hour, from: date)
        
        //if we're in night-time, set a dark background color 
        if hour > 17 || hour < 6{
            self.view.backgroundColor = UIColor.darkGray
        }
    }
    
    let numberFormatter: NumberFormatter = {
        let nf = NumberFormatter()
        nf.numberStyle = .decimal
        nf.minimumFractionDigits = 0
        nf.maximumFractionDigits = 1
        return nf
    }()
    
    func textField(_ textField: UITextField, shouldChangeCharactersIn range: NSRange, replacementString string: String) -> Bool {
        
        let currentLocale = Locale.current
        let decimalSeparator = currentLocale.decimalSeparator ?? "."
        
        let existingTextHasDecimalSeparator = textField.text?.range(of: decimalSeparator)
        let replacementTextHasDecimalSeparator = string.range(of: decimalSeparator)
        
        if existingTextHasDecimalSeparator != nil &&
            replacementTextHasDecimalSeparator != nil{
            return false
        }
        else{
            return true
        }    
    }
}
