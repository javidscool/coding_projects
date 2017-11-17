//run this function immediately
(function(){
	//remove the HTML content from the main page so we can add it to the modal window
	var $content = $('#share-options').detach();
	
	$('#share').on('click', function(){
		modal.open({content: $content, width: 340, height: 300});
	});
}());