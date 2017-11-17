(function(){
	//store all the images
	var $imgs = $('#gallery img');
	
	//store the button element
	var $buttons = $('#buttons');
	
	//create the tagged object
	var tagged = {};
	
	//go thru each of the images
	$imgs.each(function(){
		//get the image we're looking at
		var img = this;
		
		//get this image's tags
		var tags = $(this).data('tags');
		
		//if this image had tags
		if(tags){
			//create an array of tags with a comma as the delimiter and go thru all tags
			tags.split(',').forEach(function(tagName){
				//if our tagged object doesn't contain this tag...
				if(tagged[tagName] == null){
					//...add it to out array
					tagged[tagName] = [];
				}
				//now add the image to the tagged property of the array
				tagged[tagName].push(img);
			});
		}
	});
	
	
	//now let's create all the buttons and their event handlers
	//we'll start with the show all button
	$('<button/>', {
		//set button's text to 'show all'
		text: 'Show All',
		//make this button active (it's the default)
		class: 'active',
		//an an onclick handler to it
		click: function(){
			//get the clicked on button
			$(this)
				.addClass('active') //add class of active to the button
				.siblings() //select all its siblings...
				.removeClass('active'); //...and remove the active class from them
			
			//lastly, we show all the images
			$imgs.show();
		}
	}).appendTo($buttons); //append this button to the buttons div
	
	//next, we'll add the rest of the buttons
	$.each(tagged, function(tagName){
		//create the button
		$('<button/>', {
			//set button's text to the tag name and the number of pics with the tag
			text: tagName + '(' + tagged[tagName].length + ')',
			//an an onclick handler to it
			click: function(){
				//get the clicked on button
				$(this)
					.addClass('active') //add class of active to the button
					.siblings() //select all its siblings...
					.removeClass('active'); //...and remove the active class from them
				
				//lastly, we show the images with the selected tag
				$imgs
					.hide() //we initially hide all the images
					.filter(tagged[tagName]) //then, we select only the images that have this tag name...
					.show(); //...then we show them
			}
		}).appendTo($buttons); //append this button to the buttons div
	});
}());











