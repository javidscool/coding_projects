//
//  WebViewController.swift
//  WorldTrotter
//
//  Created by Javid Shamloo on 3/19/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import UIKit
import WebKit

class WebViewController : UIViewController {
    
    var webView: WKWebView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        //create our web view
        webView = WKWebView()
        
        //create our web request
        let myURL = URL(string: "https://www.bignerdranch.com")
        let myRequest = URLRequest(url: myURL!)
        
        //load our request into the web view
        webView.load(myRequest)
        
        //load the web view
        view = webView
        
    }
    
}
