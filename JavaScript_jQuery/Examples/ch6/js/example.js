var noteInput, noteName, textEntered, target;

noteName = document.getElementById('noteName');
noteInput = document.getElementById('noteInput');

function writeLabel(e){
	if(!e){
		e = window.event;
	}
	target = e.target || e.srcElement;
	textEntered = e.target.value;
	noteName.textContent = textEntered;
}

function recorderControls(e){
	if(!e){
		e = window.event;
	}
	target = e.target || e.srcElement;
	if(e.preventDefault){
		e.preventDefault();
	}
	else{
		e.returnValue = false;
	}
	
	switch(target.getAttribute('data-state')){
		case 'record':
			record(target);
			break;
		case 'stop':
			stop(target);
			break;
		//more buttons could go here
	}
}

function record(target){
	target.setAttribute('data-state', 'stop');
	target.setContent = 'stop';
}

function stop(target){
	target.setAttribute('data-state', 'record');
	target.setContent = 'record';
}

if(document.addEventListener){
	document.addEventListener('click', function(e){ recorderControls(e); }, false);
	noteInput.addEventListener('input', writeLabel, false);
}
else{
	document.attachEvent('onclick', function(e){ recorderControls(e); }, false);
	noteInput.attachEvent('onkeyup', writeLabel, false);
}
