$(document).ready(function() {
	
	var geocoder = new google.maps.Geocoder();

	document.getElementById('addrButton').addEventListener('click', function() {
		console.log("CLICK!!!");
		geocodeAddress(geocoder);
	});
  
	
	function geocodeAddress(geocoder) {
		var address = document.getElementById('address').value;
		console.log(address);
		
		geocoder.geocode({'address': address}, function(results, status) {
			if (status === 'OK') {
				console.log("Geometry location: " + results[0].geometry.location);
				document.getElementById('latitude').value = results[0].geometry.location.lat();
				document.getElementById('longitude').value = results[0].geometry.location.lng();
			} else {
				alert('Geocode was not successful for the following reason: ' + status);
			}
		});
	}
	
});