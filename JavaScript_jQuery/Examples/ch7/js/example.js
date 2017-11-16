//this is shorthand for document.ready() and waits till the DOM tree is ready before running this code
$(function(){

	//setup
	var $list, $newItemFrom, $newItemButton;
	
	//item is an empty string
	var item = '';
	
	//cache the unordered list
	$list = $('ul');
	
	//cache form to add new items (# is finding an HTML ID)
	$newItemForm = $('#newItemForm');
	
	//cache button to show form
	$newItemButton = $('#newItemButton');
	
	//this function hides the list items first and then fades them in
	$('li').hide().each(function(index){
		$(this).delay(450 * index).fadeIn(1600);
	});
	
	//this function is the item counter for items that are in the list that are NOT complete
	function updateCount(){
		var items = $('li[class!=complete]').length;
		$('#counter').text(items);
	}
	
	//call the function when we start
	updateCount();
	
	
	//setup form for new items
	
	//originally, we show the button and hide the form
	$newItemButton.show();
	$newItemForm.hide();
	//when the user clicks the button, we show the form and hide the button
	$('#showForm').on('click', function(){
		$newItemButton.hide();
		$newItemForm.show();
	});
	
	
	//event for adding a new list item
	//when the form is submitted...
	$newItemForm.on('submit', function(e){
		//...prevent the form being submitted to the server
		e.preventDefault();
		
		//get the text from the form
		var text = $('input:text').val();
		
		//add another list item to the ul using the text entered
		$list.append('<li>' + text + '</li>');
		
		//clear the text on the form
		$('input:text').val('');
		
		//update the count of the items
		updateCount();
	});
	
	
	//click handling - uses delegation on <ul> element
	//here, the second arhument means we're only responding to li elements
	$list.on('click', 'li', function(){
		var $this = $(this);
		//getting bool seeing if item is complete or not
		var complete = $this.hasClass('complete');
		
		//if the item is complete
		if(complete === true){
			//animate the list item being removed from the list
			$this.animate({
				opacity: 0.0,
				paddingLeft: '+=180'
			}, 500, 'swing', function(){
				$this.remove();
			});
		}
		else{
			//get the list item
			item = $this.text();
			//then remove it from the list
			$this.remove();
			//then append it to the bottom of the list with a class of complete and gade it in
			$list.append('<li class=\"complete\">' + item + '</li>')
				 .hide().fadeIn(300);
				 
			//lastly, update the count
			updateCount();
		}
	});
	
});