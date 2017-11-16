//this is the main validation routine
//here we run the script when the DOM is ready
$(function(){

	//when the DOM is ready, we immediately call this function and run it
	(function(){
		//the first thing we do is disable HTML5 validation...we'll take care of the validation thru JavaScript
		document.forms.register.noValidate = true;
		
		//event handler when the form is submitted
		$('form').on('submit', function(e){
			
			//get all the form control elements
			var elements = this.elements;
			
			//the custom valid object that will hold the element's id along with a bool stating whether it's valid or not
			var valid = {};
			
			//this is a re-used flag that checks each form control
			var isValid;
			
			//this is the master flag that checks the entire form
			var isFormValid;
			
			//first, we need to perform the generic checks on each element
			for(var i = 0; i < (elements.length - 1); i++){
				//call the validateRequired and validateTypes methods to perform the generic checks
				isValid = validateRequired(elements[i]) && validateTypes(elements[i]);
				
				//if the element doesn't pass these two tests...
				if(!isValid){
					//...show error message
					showErrorMessage(elements[i]);
				}
				//else we remove any error messages on the element
				else{
					removeErrorMessage(elements[i]);
				}
				//add the element and it's isValid result to the valid array object
				valid[elements[i].id] = isValid;
			}
			
			//now that we've done the generic checks, we need to perform custom validation
			//call validateBio to validate the bio element
			if(!validateBio()){
				//show error if the bio is not valid
				showErrorMessage(document.getElementById('bio'));
				
				//update the valid object
				valid.bio = false;
			}
			//else there is no error so remove it
			else{
				removeErrorMessage(document.getElementById('bio'));
			}
			
			//call validatePassword to validate the password element
			if( !validatePassword(document.getElementById('password')) ){
				//show error if the password is not valid
				showErrorMessage(document.getElementById('password'));
					
				//update the valid object
				valid.password = false;
			}
			//else there is no error so remove it
			else{
				removeErrorMessage(document.getElementById('password'));
			}
			
			//call validatePassword to validate the confirm password element
			if( !validatePassword(document.getElementById('confPassword')) ){
				//show error if the confirm password is not valid
				showErrorMessage(document.getElementById('confPassword'));
							
				//update the valid object
				valid.confPassword = false;
			}
			//else there is no error so remove it
			else{
				removeErrorMessage(document.getElementById('confPassword'));
			}
			
			//call validatePasswordMatch to make sure the passwords match
			//we first check that the passwords are ok up to this point
			if(valid.password && valid.confPassword){
				//then we check that the passwords match
				if(!validatePassMatch()){
					//show errors if the passwords do not match
					showErrorMessage(document.getElementById('password'));
					showErrorMessage(document.getElementById('confPassword'));
								
					//update the valid object
					valid.password = false;
					valid.confPassword = false;
				}
				//else there are no errors so remove them
				else{
					removeErrorMessage(document.getElementById('password'));
					removeErrorMessage(document.getElementById('confPassword'));
				}
			}
			
			//call validateParentsConsent to validate the parentsConsent element
			if(!validateParentsConsent()){
				//show error is the parental consent is not valid
				showErrorMessage(document.getElementById('parentsConsent'));
				
				//update the valid object
				valid.parentsConsent = false;
			}
			//else there is no error so remove it
			else{
				removeErrorMessage(document.getElementById('parentsConsent'));
			}
			
			//now that we've performed all the checks, we can see if validation passed and if we can submit the form
			
			//first we set isFormValid to true and wait till this value breaks and gets set to false...if it doesn't then we're cool
			isFormValid = true;
			//we check this by looping thru all the objects in valid and seeing if they caught any errors
			for(var field in valid){
				//if we did catch an error...
				if(!valid[field]){
					//...set isFormValid to false and stop the loop
					isFormValid = false;
					break;
				}
			}
			
			//if the form is not valid, don't submit it
			if(!isFormValid){
				e.preventDefault();
			}
			
			//else we submit (here we just throw an alert to the screen)
			//DEMO PURPOSES ONLY (WE SHOULD NOT DO e.preventDefault())
			else{
				e.preventDefault();
				alert('Form submitted successfully!');
			}
			
		});
		
		
		//function checks if the element has the required attribute and a value
		function validateRequired(el){
			//first we check that the field is required
			if(isRequired(el)){
				//if it is we check if it has a value
				var valid = !isEmpty(el);
				//if there is no value then we show an error message
				if(!valid){
					setErrorMessage(el, 'Field is required');
				}
				return valid;
			}
			//if there is no required field for this element, we're cool on this end
			return true;
		}
		
		//function to check if the element has a required attribute
		function isRequired(el){
			//this function uses typeof to check what datatype the browser thinks the required attribute is
			
			//the first part of this return statement is for modern browsers, the second is for older browsers 
			return ( (typeof el.required === 'boolean') && el.required) || (typeof el.required === 'string');
		}
		
		//function to check whether the element has a value
		function isEmpty(el){
			//the first part of this return statement is for modern browsers, the second is for older browsers (cause polyfill place-holder text will match the value of the element if it's empty)
			return !el.value || el.value === el.placeholder;
		}
		
		//this function adds a data- attribute of data-errorMessage to the element that has an error
		//the value of the attribute contains the error message itself
		function setErrorMessage(el, message){
			$(el).data('errorMessage', message);
		}
		
		
		//this function removes the error message
		function removeErrorMessage(el) {
			//check if we have an associated <span> element for the error message
			var errorContainer = $(el).siblings('.error.message');
			
			//if we have an error message, remove it
			if(errorContainer.length){
				errorContainer.remove();
			}
		}
		
		//this function shows the error message we placed in the data- attribute
		function showErrorMessage(el){
			//get element with the error
			var $el = $(el);
			
			//check if we've already added a <span> element with a class of error to the element we're looking at (it'll be a sibling of it)
			var errorContainer = $el.siblings('.error.message');
			
			//if we don't have an error associated with this element...
			if(!errorContainer.length){
				//...create a <span> to hold the error and add it after the element with the error
				errorContainer = $('<span class="error message"></span>').insertAfter($el);
			}
			
			//add the error message to the span
			errorContainer.text($(el).data('errorMessage'));
		}
		
		
		//this function validates the type of input entered to make sure it's of the correct format
		function validateTypes(el){
			//if this element has no input, return true because there's nothing to check
			if(!el.value) return true;
			
			//if we have a value, see what type it is
			//we use .data() because we have a function that will store the type in a data- attribute and we need to check for this
			//also, getAttribute() is used because all browsers understand it and return the right thing from it
			var type = $(el).data('type') || el.getAttribute('type');
			
			//check if we have a method in our validateType object to check this type
			if(typeof validateType[type] === 'function'){
				//if we do have a function to validate this type, use it
				return validateType[type](el);
			}
			//if we don't have a function to validate the type, return true
			//THIS IS DANGEROUS CAUSE SHIT COULD STILL BE FUCKED UP!!!  MAKE SURE ALL TYPES YOU USE HAVE METHODS TO CHECK FOR VALIDATION
			else{
				return true;
			}
		}
		
		//this object will contain all the validation methods for the types we use on this form
		//we use regular expressions for all these functions to check that the pattern of the types is correct
		var validateType = 
		{
			//function to validate email
			email: function(el){
				//this is the reg expression to check emails
				var valid = /[^@]+@[^@]+/.test(el.value);
				
				//if the value the user entered is not matching the pattern, throw an error message
				if(!valid){
					setErrorMessage(el, 'Please enter a valid email');
				}
				return valid;
			},
			//function to validate numbers
			number: function(el){
				//this is the reg expression to check numbers
				var valid = /^\d+$/.test(el.value);
				
				//if the value the user entered is not matching the pattern, throw an error message
				if(!valid){
					setErrorMessage(el, 'Please enter a valid number');
				}
				return valid;
			},
			//function to validate dates
			date: function(el){
				//this is the reg expression to check dates
				var valid = /^(\d{2}\/\d{2}\/\d{4})|(\d{4}-\d{2}-\d{2})$/.test(el.value);
				
				//if the value the user entered is not matching the pattern, throw an error message
				if(!valid){
					setErrorMessage(el, 'Please enter a valid date');
				}
				return valid;
			}
		};
		
		//this function checks if the bio is 140 characters or less
		function validateBio(){
			//get the bio element from the form
			var bio = document.getElementById('bio');
			
			//checking if the bio is less than 140 characters
			var valid = (bio.value.length <= 140);
			
			//if the bio is more than 140 characters, set the error message
			if(!valid){
				setErrorMessage(bio, 'Your bio should not exceed 140 characters');
			}
			
			return valid;
		}
		
		//this function checks if a passed-in password has at least 8 characters
		function validatePassword(el){
			//first check that the password is at least 8 characters
			var valid = (el.value.length >= 8);
			
			//if the password is not at least 8 characters, set an error message on it
			if(!valid){
				setErrorMessage(el, 'Password must be at least 8 characters');
			}
			
			return valid;
		}
		
		//this function checks if the password and confirm password match
		function validatePassMatch(){
			//get the two passwords
			var password = document.getElementById('password');
			var confPassword = document.getElementById('confPassword');
			
			var valid = true;
			
			//check that the passwords match
			//if they don't, set error messages
			if(password.value !== confPassword.value){
				setErrorMessage(password, 'Passwords must match');
				setErrorMessage(confPassword, 'Passwords must match');
				
				valid = false;
			}
			
			return valid;
		}
		
		//this function checks to see if the user gave parental consent or not
		function validateParentsConsent(){
			//get the elements for the parental checkbox and it's container
			var parentsConsent = document.getElementById('parentsConsent');
			var consentContainer = document.getElementById('consentContainer');
			
			//set the valid variable initially to true
			var valid = true;
			
			//check if the consent container is NOT hidden
			if(consentContainer.className.indexOf('hide') === -1){
				//get whether the checkbox is checked or not
				valid = parentsConsent.checked;
				
				//if the box is not checked, we have an error
				if(!valid){
					setErrorMessage(parentsConsent, 'You need your parents\' consent');
				}
			}
			
			return valid;
		}
		
	}());
	
});