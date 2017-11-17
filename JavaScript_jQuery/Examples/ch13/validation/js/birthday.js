(function(){
	//get the birthday input
	var $birth = $('#birthday');
	
	//get the checkbox for parentsConsent
	var $parentsConsent = $('#parentsConsent');
	
	//get the container for the consent text and checkbox
	var $consentContainer = $('#consentContainer');

	//create the date picker using jQuery UI
	//.prop changes the date input from date format to text so that we don't conflict with HTML5 date picker functionality
	$birth.prop('type', 'text').data('type', 'date').datepicker({
		dateFormat: 'yy-mm-dd'
	});

	//when we blur or lose focus on the date we picked, call the checkDate function
	$birth.on('blur change', checkDate);

	//this function creates a date object with what we entered and calls toggleParentsConsent to do show or hide the consent container
	function checkDate(){
		//get an array from the date we entered
		var dob = this.value.split('-');
		
		//Pass toggleParentsConsent the date of birth as a date object (we write (dob[1] - 1) because months are given from 0-11)
		toggleParentsConsent(new Date(dob[0], dob[1] - 1, dob[2]));
	}

	//this function determines whether or not we show the consent container (which contains the checkbox)
	function toggleParentsConsent(date){
		//if we get invalid input, just return
		if (isNaN(date)) return;
		
		//get the date right now by creating a new date object
		var now = new Date();
		
		//if the user is less than 13 we show parents consent checkbox
		//time is in milliseconds
		if ( (now - date) < (1000 * 60 * 60 * 24 * 365 * 13) ){ 
			//remove the hide class to show the container
			$consentContainer.removeClass('hide');
			//also, give the container focus
			$parentsConsent.focus();
		} 
		//else we hide the container
		else{
			//add a class of hide to hide the container
			$consentContainer.addClass('hide');
			//uncheck its checkbox
			$parentsConsent.prop('checked', false);
		}
	}
	
}());