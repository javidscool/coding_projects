//
//  MapViewController.swift
//  WorldTrotter
//
//  Created by Javid Shamloo on 2/23/17.
//  Copyright © 2017 BigNerdRanch. All rights reserved.
//

import UIKit
import MapKit

//I finished the bronze challenge from chapter 4
//I finished the silver challenge from chapter 5
//I finished the bronze, silver, and gold challenges from chapter 6
//I finished the bronze challenge from chapter 7

//struct that holds our pin locations
struct Location{
    var name: String
    var lat: Double
    var long: Double
    
    init(name: String, lat: Double, long: Double){
        self.name = name
        self.lat = lat
        self.long = long
    }
}

class MapViewController: UIViewController, MKMapViewDelegate, CLLocationManagerDelegate{
    
    var mapView: MKMapView!
    var locationManager: CLLocationManager!
    
    var selectedAnnotationIndex: Int = 0
    
    override func loadView(){
        //create a map view
        mapView = MKMapView()
        
        //set this view controller as our map-view delegate and create a location manager
        mapView.delegate = self
        locationManager = CLLocationManager()
        
        //set it as the view of this view controller
        view = mapView
        
        let standardString = NSLocalizedString("Standard", comment: "Standard map view")
        let hybridString = NSLocalizedString("Hybrid", comment: "Hybrid map view")
        let satelliteString = NSLocalizedString("Satellite", comment: "Satellite map view")
        
        let segmentedControl = UISegmentedControl(items: [standardString, hybridString, satelliteString])
        
        segmentedControl.backgroundColor =
            UIColor.white.withAlphaComponent(0.5)
        segmentedControl.selectedSegmentIndex = 0
        
        segmentedControl.addTarget(self,
        action: #selector(MapViewController.mapTypeChanged(_:)),
        for: .valueChanged)
        
        segmentedControl.translatesAutoresizingMaskIntoConstraints =
        false
        view.addSubview(segmentedControl)
        
        let topConstraint = segmentedControl.topAnchor.constraint(
            equalTo: topLayoutGuide.bottomAnchor, constant: 8)
        
        let margins = view.layoutMarginsGuide
        
        let leadingConstraint = segmentedControl.leadingAnchor.constraint(
            equalTo: margins.leadingAnchor)
        
        let trailingConstraint = segmentedControl.trailingAnchor.constraint(
            equalTo: margins.trailingAnchor)
        
        topConstraint.isActive = true
        leadingConstraint.isActive = true
        trailingConstraint.isActive = true
        
        
        //create our button
        initLocalizationButton(segmentedControl)
        
        
        //create our button to toggle between the pins we will put down
        initPinButton(bottomLayoutGuide, margins)
        
        //next we create our locations and drop them onto the map
        createPinLocations()
    }
    
    func initPinButton(_ bottomLayoutGuide: UILayoutSupport, _ margins: UILayoutGuide){
        //create our pin button
        let pinButton = UIButton.init(type: .system)
        pinButton.setTitle("Pins", for: .normal)
        pinButton.backgroundColor = UIColor.white.withAlphaComponent(0.5)
        pinButton.frame = CGRect(x: 0, y: 0, width: 100, height: 50)
        pinButton.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(pinButton)
        
        //next, we create our constraints and enable them
        let bottomConstraint =
            pinButton.bottomAnchor.constraint(equalTo: bottomLayoutGuide.topAnchor, constant: -8)
        let leftConstraint = pinButton.leftAnchor.constraint(equalTo: margins.leftAnchor)
        
        bottomConstraint.isActive = true
        leftConstraint.isActive = true
        
        //then we add a method to call when this button is clicked
        pinButton.addTarget(self,
                            action: #selector(MapViewController.selectPin(_:)),
                            for: .touchUpInside)
    }
    
    func selectPin(_ button: UIButton){
        //first check if we even have annotations to display, and return if we don't
        if !(mapView.annotations.count > 0){
            return
        }
        
        //next we select our pin and display it
        let annotation = mapView.annotations[selectedAnnotationIndex]
        let zoomedInCurrentLocation = MKCoordinateRegionMakeWithDistance(
            annotation.coordinate, 1000, 1000)
        //set the region to this zoomed-in user's location
        mapView.setRegion(zoomedInCurrentLocation, animated: true)
        
        //finally we cycle thru our annotations
        selectedAnnotationIndex += 1
        if selectedAnnotationIndex >= mapView.annotations.count{
            selectedAnnotationIndex = 0
        }
    }
    
    func createPinLocations(){
        //first we create an array of location objects andf append locations to it
        var locations = [Location]()
        locations.append(Location(name: "Miami", lat: 25.761681, long: -80.191788))
        locations.append(Location(name: "Orlando", lat: 28.538336, long: -81.379234))
        locations.append(Location(name: "Waikiki", lat: 21.290014, long: -157.835938))
        
        //next, we drop the pins onto the map
        for location in locations{
            let dropPoint = MKPointAnnotation()
            dropPoint.coordinate = CLLocationCoordinate2DMake(location.lat, location.long)
            dropPoint.title = location.name
            mapView.addAnnotation(dropPoint)
        }
    }
    
    //this function returns the view associated with the specific annotation object
    func mapView(_ mapView: MKMapView, viewFor annotation: MKAnnotation) -> MKAnnotationView? {
        
        if annotation is MKUserLocation{
            //return nuil so we draw a blue dot for the user's location
            return nil
        }
        
        //first try to see if we can get a pin view for the annotation
        var pinView =
            mapView.dequeueReusableAnnotationView(withIdentifier: "pin") as? MKPinAnnotationView
        
        //if we can't we need to create a pin view for this annotation
        if pinView == nil{
            pinView = MKPinAnnotationView(annotation: annotation, reuseIdentifier: "pin")
            
            //this indicates whether the annotation view is able 
            //to display extra information in a callout bubble
            pinView!.canShowCallout = true
            
            //set this to true to animate the view onto the screen
            pinView!.animatesDrop = true
            
            pinView!.pinTintColor = .red
        }
        else{
            //else we have a pin already configured, so get it
            pinView!.annotation = annotation
        }
        
        return pinView
    }
    
    //this function will let us programatically create our button and add an action method
    //to it when it's clicked
    func initLocalizationButton(_ anyView: UIView!){
        //first we create our button, set its title, and add it to our subview
        let localizationButton = UIButton.init(type: .system)
        localizationButton.setTitle("Localization", for: .normal)
        localizationButton.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(localizationButton)
        
        //next, we add our button's constraints and enable them
        let topConstraint = localizationButton.topAnchor.constraint(
            equalTo: anyView.topAnchor, constant: 32)
        let leadingConstraint = localizationButton.leadingAnchor.constraint(
            equalTo: view.layoutMarginsGuide.leadingAnchor)
        let trailingConstraint = localizationButton.trailingAnchor.constraint(
            equalTo: view.layoutMarginsGuide.trailingAnchor)
        
        topConstraint.isActive = true
        leadingConstraint.isActive = true
        trailingConstraint.isActive = true
        
        //then we add a method to call when this button is clicked
        localizationButton.addTarget(self,
                                     action: #selector(MapViewController.showLocalization(sender:)),
                                     for: .touchUpInside)
        
    }
    
    func showLocalization(sender: UIButton!){
        //first we request permission to use the location services
        locationManager.requestWhenInUseAuthorization()
        
        //then state that the map should try to display the user's location
        mapView.showsUserLocation = true
        
        locationManager.stopUpdatingLocation()
        locationManager.startUpdatingLocation()
    }
    
    //this tells our delegate that the users location was updated
    func mapView(_ mapView: MKMapView, didUpdate userLocation: MKUserLocation) {
        //we create a region with the specified coordinate and distance values that indicate
        //the north-to-south and east-to-west span, respectively
        let zoomedInCurrentLocation = MKCoordinateRegionMakeWithDistance(
            userLocation.coordinate, 500, 500)
        
        //then we set the region to this zoomed-in user's location
        mapView.setRegion(zoomedInCurrentLocation, animated: true)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
    }
    
    func mapTypeChanged(_ segControl: UISegmentedControl){
        switch segControl.selectedSegmentIndex{
        case 0:
            mapView.mapType = .standard
        case 1:
            mapView.mapType = .hybrid
        case 2:
            mapView.mapType = .satellite
        default:
            break
        }
    }
}
