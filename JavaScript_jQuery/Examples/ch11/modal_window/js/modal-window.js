//using an immediately invoked function expression to create the object immediately
var modal = (function(){
	//get the window object of the page
	var $window = $(window);
	
	//create the markup for the modal window
	var $modal = $('<div class="modal" />');
	var $content = $('<div class="modal-content" />');
	var $close = $('<button role="button" class="modal-close">close</button>');
	
	//we're creating an overlay element that will darken the rest of the screen when the modal window pops up
	$overlay = $('<div class="overlay"/>');     
	
	//add all the content together
	$modal.append($content, $close);
	
	//create method for close button
	$close.on('click', function(e){
		//prevent link behavior
		e.preventDefault();
		
		//call the close method defined below
		modal.close();
	});
	
	//this return method will return these three functions to use with the modal object
	return{
		//function to center the window
		center: function(){
			//calculate distance from top and left of window to center the modal
			var top = Math.max($window.height() - $modal.outerHeight(), 0) / 2;
			var left = Math.max($window.width() - $modal.outerWidth(), 0) / 2;
			
			//center the modal window within the page
			$modal.css({
				top : top + $window.scrollTop(),
				left: left + $window.scrollLeft()
			});
		},
		//function to open the window
		open: function(settings){
			//empty what was in the contents of the modal before and set the contents of the modal
			$content.empty().append(settings.content);
			
			//set modal dimensions if they were given
			$modal.css({
				width: settings.width || 'auto',
				height: settings.height || 'auto'
			}).appendTo('body');
			
			//center the window
			modal.center();
			
			//create event to center the window if it's resized
			$(window).on('resize', modal.center);
			
			//adding the overlay after the modal element to make the background screen darken and to add an effect to the close button when we hover over it
			$('.modal').after($overlay);
		},
		//function to close the window
		close: function(){
			//remove the content from the modal window
			$content.empty();

			//remove the modal window from the page
			$modal.detach();
			
			//remove the overlay
			$overlay.detach();                         
			
			//remove the modal event handler
			$(window).off('resize', modal.center);
		}
	};
}());