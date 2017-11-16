(function(){
	//get the password and confirm password inputs
	var password = document.getElementById('password');
	var passwordConfirm = document.getElementById('confPassword');
	
	//this function sets whether the passwords are good or bad
	function setErrorHighlighter(e) {
		//get the target element
		var target = e.target || e.srcElement;
		
		//if its length is less than 8 we fail
		if(target.value.length < 8){
			target.className = 'fail';
		}
		//else we're good
		else{
			target.className = 'pass';
		}
	}

	//this function removes the error highlighting (if it's there)
	function removeErrorHighlighter(e){
		//get the target element
		var target = e.target || e.srcElement;
		
		//if the class is set to fail clear it
		if(target.className === 'fail'){
			target.className = '';
		}
	}

	//this function checks that the passwords match and that the confirm password is more than 8 characters
	function passwordsMatch(e) {
		//get the target element
		var target = e.target || e.srcElement;
		
		//if out target is of sufficient length
		if(target.value.length >= 8){
			//if our target is the password field, and the confirm password has a value, check that it's the same as the confirm password field
			if( (target.id === "password") && passwordConfirm.value){
				//if the passwords match we're cool
				if(target.value === passwordConfirm.value){ 
					target.className = 'pass';
					passwordConfirm.className = 'pass';
				} 
				//else we fail
				else{
					target.className = 'fail';
					passwordConfirm.className = 'fail';
				}
			}
			
			//else our target is the confirm password field, so check that it's the same as the password field and that the password field has a value
			else if( (target.id === "confPassword") && password.value){
				//if the passwords match we're cool
				if(target.value === password.value){ 
					target.className = 'pass';
					password.className = 'pass';
				} 
				//else we fail
				else{
					target.className = 'fail';
					password.className = 'fail';
				}
			}
			
			//if none of our boxes have a password
			else if(!password.value && !passwordConfirm.value){
				password.className = '';
				passwordConfirm.className = '';
			}
		}
		//if our target is too short, we fail
		else{
			target.className = 'fail';
		}
	}

	//add event listeners for these events
	addEvent(password, 'blur', setErrorHighlighter);
	addEvent(passwordConfirm, 'blur', setErrorHighlighter);
	
	addEvent(password, 'focus', removeErrorHighlighter); 
	addEvent(passwordConfirm, 'focus', removeErrorHighlighter);
	
	addEvent(password, 'blur', passwordsMatch);
	addEvent(passwordConfirm, 'blur', passwordsMatch);
	
}());