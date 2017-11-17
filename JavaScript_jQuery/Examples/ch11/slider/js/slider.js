//for every slider we set things up like this
$('.slider').each(function(){
	//get the current slider
	var $this = $(this);
	
	//get the container that holds all the slides
	var $group = $this.find('.slide-group');
	
	//get all the individual slides themselves
	var $slides = $this.find('.slide');
	
	//create array to hold nav buttons
	var buttonArray = [];
	
	//the index number of the current slide (it inits to 0, the first slide)
	var currentIndex = 0;
	
	//to store the timer
	var timeout;
	
	//this move function will animate from the slider you're at now to the slider you, or the next one in the timer, selects
	function move(newIndex){
		//animate from left or right
		var animateLeft;
		
		//position new slide to left or right of current one
		var slideLeft;
		
		//this will clear out the timer and give the user a fresh four seconds so the slider doesn't move early
		advance();
		
		//if the slide is animating or the current slide is showing, then do nothing
		if($group.is(':animated') || currentIndex === newIndex){
			return;
		}
		
		//remove the active class from current button
		buttonArray[currentIndex].removeClass('active');
		
		//add the active class to the new button
		buttonArray[newIndex].addClass('active');
		
		//if the new index > current index
		if(newIndex > currentIndex){
			//sit new slide to the right of current one
			slideLeft = '100%';
			
			//move the current slide to the left
			animateLeft = '-100%';
		}
		else{
			//sit new slide to the left of current one
			slideLeft = '-100%';
			
			//move the current slide to the right
			animateLeft = '100%';
		}
		
		//position new slide and display it
		$slides.eq(newIndex).css( {left: slideLeft, display: 'block'} );
		
		//animate the slides and do some cleanup work
		$group.animate( {left: animateLeft}, function(){
			//hide the current slide after we animate it out
			$slides.eq(currentIndex).css( {display: 'none'} );
			
			//left-align the new slide
			$slides.eq(newIndex).css( {left: 0} );
			
			//left-align the rest of the slides
			$group.css( {left: 0} );
			
			//set the currentIndex to the new image
			currentIndex = newIndex;
		});
	}
	
	//this function creates the timer that will call move() that will move the slides when the user isn't doing shit
	function advance(){
		//clear the timer
		clearTimeout(timeout);
		
		//start timer to run an anonymous function every 4 seconds
		timeout = setTimeout(function(){
			//if the current index is less than the last index, move to the next slide
			if(currentIndex < ($slides.length - 1)){
				move(currentIndex + 1);
			}
			//else, move from the last slide to the first one
			else{
				move(0);
			}
		}, 4000); //set the timer to move every 4 secs
	}
	
	//here we're moving through each of the slides and adding a button to navigate to each of them and putting an event handler to each button we create
	$.each($slides, function(index){
		//create a button element
		var $button = $('<button type="button" class="slide-btn">&bull;</button>');
		
		//if the index is the current item, we make it active
		if(index === currentIndex){
			$button.addClass('active');
		}
		//create event handler for the button
		$button.on('click', function(){
			//it moves to where we want
			move(index);
		}).appendTo('.slide-buttons'); //we add it to the HTML div for the buttons
		
		//we also add this button to the button array
		buttonArray.push($button);
	});
	
	//call advance to start the timer
	advance();
});










