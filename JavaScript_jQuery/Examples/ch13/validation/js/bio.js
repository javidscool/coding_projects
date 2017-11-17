(function(){
	//get the bio and bio-count elements
	var bio = $('#bio');
	var bioCounter = $('#bio-count');

	//show the counter when the field is focused and update the class
	//depending on amount of characters left
	bio.on('focus', updateCounter);
	bio.on('keyup', updateCounter);

	//when we leave the textarea, we hide the counter unless there are too
	//many characters
	bio.on('blur', function(){
		if (bioCounter.text() >= 0) {
			bioCounter.addClass('hide');
		}
	});

	//this function shows the count of characters left and gives a CSS visual warning when we get close/over the limit
	function updateCounter(e){
		//first get the number of characters we have left
		var count = 140 - bio.val().length;
		
		var status = '';
		
		//if our count of characters left is less than 0, we've used too much and so our class should be error
		if (count < 0){
			status = 'error';
		} 
		//else if we're getting close set a class of warn
		else if (count <= 15) {
			status = 'warn';
		} 
		//else we're cool so set a class of good
		else {
			status = 'good';
		}

		//we remove the previous classes before we set the class again
		bioCounter.removeClass('error warn good hide');
    
		//add new class to the bio-counter and set the text to the count so the user can see
		bioCounter.addClass(status);
		bioCounter.text(count);
	}

}());
