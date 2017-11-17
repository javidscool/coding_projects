//
//  ViewController.swift
//  VocalTextEdit
//
//  Created by Javid Shamloo on 2/7/16.
//  Copyright © 2016 BigNerdRanch. All rights reserved.
//

import Cocoa

class ViewController: NSViewController, NSSpeechSynthesizerDelegate {
    
    //I SOLVED THE SILVER AND GOLD CHALLENGES
    
    let speechSynthesizer = NSSpeechSynthesizer()
    
    @IBOutlet var textView: NSTextView!
    @IBOutlet var speakButton: NSButton!
    @IBOutlet var stopButton: NSButton!
    @IBOutlet var progressIndicator: NSProgressIndicator!
    
    var wordsToSpeak: Int = 0
    
    var contents: String?{
        get{
            return textView.string
        }
        set{
            textView.string = newValue
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        speechSynthesizer.delegate = self
        
        progressIndicator.isHidden = true
        speechSynthesizer.rate = 200
    }
    
    func speechSynthesizer(_ sender: NSSpeechSynthesizer, didFinishSpeaking finishedSpeaking: Bool) {
        speakButton.isEnabled = true
        stopButton.isEnabled = false
        
        progressIndicator.isHidden = true
        progressIndicator.doubleValue = 0.0
    }
    
    func speechSynthesizer(_ sender: NSSpeechSynthesizer, willSpeakWord characterRange: NSRange, of string: String) {
        
        if let contents = textView.string, !contents.isEmpty{
            
            //every time we're about to speak a word, increment the progress indicator by 
            //one word
            let increment = progressIndicator.maxValue / Double(wordsToSpeak)
            progressIndicator.increment(by: increment)
        }
    }
    
    func determineSpeechDuration(text: String) -> Double{
        
        //figure out the number of words spoken and set a variable holding that
        let wordsSpoken = text.characters.split(separator: " ").map(String.init)
        wordsToSpeak = wordsSpoken.count
        
        let approxTimeInSecs = Double(wordsSpoken.count) / Double(speechSynthesizer.rate) * 60
        return approxTimeInSecs
    }
    
    @IBAction func speakButtonClicked(_ sender: NSButton){
    
        if let contents = textView.string, !contents.isEmpty{
            stopButton.isEnabled = true
            speakButton.isEnabled = false
            
            //let availVoices: [String] = NSSpeechSynthesizer.availableVoices()
            //print(availVoices)
            speechSynthesizer.setVoice("com.apple.speech.synthesis.voice.Zarvox")
            
            progressIndicator.maxValue = determineSpeechDuration(text: contents)
            progressIndicator.isHidden = false
            speechSynthesizer.startSpeaking(contents)
        }
        else{
            speechSynthesizer.startSpeaking("The document is empty.")
        }
        
    }
    
    @IBAction func stopButtonClicked(_ sender: NSButton){

        speakButton.isEnabled = true
        stopButton.isEnabled = false
        
        progressIndicator.isHidden = true
        progressIndicator.doubleValue = 0.0
        
        speechSynthesizer.stopSpeaking()
    }
}

