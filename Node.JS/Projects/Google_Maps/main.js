// main.js


function startMap() {
	
	var directionsService = new google.maps.DirectionsService;
	var directionsDisplay = new google.maps.DirectionsRenderer;
	
	var ironhackBCN = {
		lat: 41.3977381, 
		lng: 2.190471916
	};
  
	var map = new google.maps.Map(
		document.getElementById('map'), 
		{
			zoom: 15,
			center: ironhackBCN
		}
	);
	
	var myMarker = new google.maps.Marker({
		position: {
			lat: 41.3977381, 
			lng: 2.190471916
		},
		map: map,
		title: "Barcelona Campus"
	});
	
	if (navigator.geolocation) {
		navigator.geolocation.getCurrentPosition(function(position) {
			const user_location = {
				lat: position.coords.latitude,
				lng: position.coords.longitude
			};
      
			// Center map with user location
			map.setCenter(user_location);
      
			// Add a marker for your user location
			var ironHackBCNMarker = new google.maps.Marker({
				position: {
					lat: user_location.lat, 
					lng: user_location.lng
				},
				map: map,
				title: "You are here"
			});
      
		}, function () {
			console.log('Error in the geolocation service.');
		});
	} else {
		console.log('Browser does not support geolocation.');
	}
	
	var directionRequest = {
		origin: { lat: 41.3977381, lng: 2.190471916},
		destination: 'Madrid, es',
		travelMode: 'DRIVING'
	};

	directionsService.route(
		directionRequest, 
		function(response, status) {
			if (status === 'OK') {
				// everything is ok
				directionsDisplay.setDirections(response);
			} else {
				// something went wrong
				window.alert('Directions request failed due to ' + status);
			}
		}
	);

	directionsDisplay.setMap(map);
}

startMap();