$(function(){
	
	var times;
	//setup the request
	$.ajax({
		//before requesting data, check if this function is supported and if yes, call it
		//it indicates that the response from the server should be treated as JSON data
		beforeSend: function(xhr){
			if(xhr.overrideMimeType){
				xhr.overrideMimeType("application/json");
			}
		}
	});
	
	
	function loadTimetable(){
		//try to collect the JSON data
		$.getJSON('data/example.json')
			.done(function(data){
				times = data;
			})
			.fail(function(){
				$('#event').html('Sorry!  We could not load the timetable at the moment');
			});
	}
	
	loadTimetable();
	
	
	//click on an event in the right hand column to load a timetable
	$('#content').on('click', '#event a', function(e){
		
		//stop the link from going anywhere, cause we're gonna update the page with AJAX right here
		e.preventDefault();
		
		var loc = this.id.toUpperCase();
		
		var newContent = '';
		for(var i = 0; i < times[loc].length; i++){
			newContent += '<li><span class="time">' + times[loc][i].time + '</span>';
			newContent += '<a href="descriptions.html#';
			//does a global replacement of all the title's spaces with a dash
			newContent += times[loc][i].title.replace(/ /g, '-') + '">';
			newContent += times[loc][i].title + '</a></li>';
		}
		
		$('#sessions').html('<ul>' + newContent + '</ul>');
		
		$('#event a.current').removeClass('current');
		$(this).addClass('current');
		
		$('#details').text('');
	});
	
	
	//click on a session time to load the description for it
	$('#content').on('click', '#sessions li a', function(e){
		e.preventDefault();
		var fragment = this.href;
		
		//adding a space before the # so the request goes properly into load()
		fragment = fragment.replace('#', ' #');
		$('#details').load(fragment);
		
		$('#sessions a.current').removeClass('current');
		$(this).addClass('current');
	});
	
	
	//click on primary navigation on top right of page
	$('nav a').on('click', function(e){
		e.preventDefault();
		var url = this.href;
		
		$('nav a.current').removeClass('current');
		$(this).addClass('current');
		
		$('#container').remove();
		$('#content').load(url + ' #container').hide().fadeIn('slow');
	});

});