(function(){
	//get all the images
	var $imgs = $('#gallery img');
	
	//get the input element
	var $search = $('#filter-search');
	
	//create an array called cache
	var cache = [];
	
	//go thru each image
	$imgs.each(function(){
		//add an object to the cache array, storing the img element and it's alt text, trimmed and put to all lowercase
		cache.push({
			element: this,
			text: this.alt.trim().toLowerCase()
		});
	});
	
	//this filter function will show/hide the images based on what you enter into the search box
	function filter(){
		//get the value of the search query
		var query = this.value.trim().toLowerCase();
		
		//go thru each of the images in the cache
		cache.forEach(function(img){
			var index = 0;
			
			//if we got a query
			if(query){
				//see if it's in out alt text
				index = img.text.indexOf(query);
			}
			
			//if we got a result (index is NOT -1) then show the image by setting its display to a blank string / else, hide the image
			img.element.style.display = index === -1 ? 'none' : '';
		});
	}
	
	//if the browser supports the input event, we use it
	if('oninput' in $search[0]){
		$search.on('input', filter);
	}
	//else, we use the keyup event
	else{
		$search.on('keyup', filter);
	}
}());