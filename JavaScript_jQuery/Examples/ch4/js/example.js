var table = 5;
var operator = 'ass';
var i = 1;
var msg = '';

if(operator === 'addition'){
	while(i < 11){
		msg += i + ' + ' + table + ' = ' + (i + table) + '<br />';
		i++;
	}
}
else{
	while(i < 11){
		msg += i + ' x ' + table + ' = ' + (i * table) + '<br />';
		i++;
	}
}

//write the message into the page
var el = document.getElementById('blackboard');
el.innerHTML = msg;