//global variable of latest image to be requested
var request;

//image currently being shown
var $current;

//the cache object
var cache = {};

//container for image
var $frame = $('#photo-viewer');

//container for thumbnails
var $thumbs = $('.thumb');

//function to crossfade between images
function crossfade($img){
	
	//if there is currently an image showing...
	if($current){
		//stop its animation and fade it out
		$current.stop().fadeOut('slow');
	}
	
	//set the CSS margins to center the image
	$img.css({
		marginLeft: -$img.width() / 2,
		marginTop: -$img.height() / 2
	});
	
	//stop animation on new image and fade it in (in case it's fading out and user clicks back to it)
	$img.stop().fadeTo('slow', 1);
	
	//new image becomes the current image
	$current = $img;
}

//event when a thumbnail is clicked on
$(document).on('click', '.thumb', function(e){
	//create local variable called $img
	var $img;
	
	//store path to image
	var src = this.href;
	
	//store path again in request
	request = src;
	
	//stop default link behavior
	e.preventDefault();
	
	//remove active from all thumbs
	$thumbs.removeClass('active');
	
	//add active class to clicked thumbnail
	$(this).addClass('active');
	
	//if the cache already contains this image
	if(cache.hasOwnProperty(src)){
		//and if isLoading is false
		if(cache[src].isLoading === false){
			//call the crossfade function to load the image
			crossfade(cache[src].$img);
		}
	}
	//else it's not in the cache
	else{
		//we first need to create an empty <img> element
		$img = $('<img/>');
		
		//store this image in the cache
		cache[src] = {
			$img: $img,
			isLoading: true
		};
		
		//this defines the event that runs when an image has loaded
		//KEEP IN MIND THE IMAGE HASN'T LOADED YET BECAUSE WE HAVEN'T GIVEN IT A SRC ATTRIBUTE YET...IT'S BLANK RIGHT NOW
		$img.on('load', function(){
			//first hide the newly loaded image
			$img.hide();
			
			//remove is-loading class from frame and append new image to it
			$frame.removeClass('is-loading').append($img);
			//update isLoading in cache
			cache[src].isLoading = false;
			
			//if this image is still the most recently requested image...
			if(request === src){
				//...load it in with a crossfade
				crossfade($img);
			}
		});
		
		//add is-loading class to frame
		$frame.addClass('is-loading');
		
		//here we set the attributes of the empty <img> element
		//after we set these attributes, the image will start to load
		$img.attr({
			'src': src,
			'alt': this.title || ''
		});
	}
});

//this line simulates clicking the first thumbnail when program first runs
$('.thumb').eq(0).click();

















