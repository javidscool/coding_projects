//find list of tabs
$('.tab-list').each(function(){
	//store the list
	var $this = $(this);
	
	//get the active list item
	var $tab = $this.find('li.active');
	
	//get the link from the active tab
	var $link = $tab.find('a');
	
	//get the active panel associated with this tab
	var $panel = $($link.attr('href'));
	
	$this.on('click', '.tab-control', function(e){
		e.preventDefault();
		
		//get the current link
		var $link = $(this);
		
		//get link's href variable
		var id = this.hash;
		
		//if the href exists and the link the user clicked is not active
		if(id && !$link.is('.active')){
			//make the previous tab and panel inactive
			$panel.removeClass('active');
			$tab.removeClass('active');
			
			//make the tab user clicked on and its associated panel active
			$panel = $(id).addClass('active');
			$tab = $link.parent().addClass('active');
		}
		
	});
});