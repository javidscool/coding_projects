(function(){
	
	//store each person as an object in an array
	//each person is an object that holds name and rate
	var people = [
		{                                             
		  name: 'Casey',                               
		  rate: 60
		},
		{
		  name: 'Camille',
		  rate: 80
		},
		{
		  name: 'Gordon',
		  rate: 75
		},
		{
		  name: 'Nigel',
		  rate: 120
		}
	];
	
	//the rows array that we'll populate
	var rows = [];
	
	//the minimum input of slider
	var $min = $('#value-min');
	
	//the maximum input of slider
	var $max = $('#value-max');
	
	//the table that shows the results
	var $table = $('#rates');
	
	//this function creates the table rows and the cross-reference array that holds each person and row in the table
	function makeRows(){
		//go through all the people
		people.forEach(function(person){
			//create a row for each person
			var $row = $('<tr></tr>');
			
			//add their name and rate to the row
			$row.append( $('<td></td>').text(person.name) );
			$row.append( $('<td></td>').text(person.rate) );
			
			//now create an cross-reference object and add it to the rows array
			rows.push({
				person: person,
				$element: $row
			});
		});
	}
	
	//this function adds all the rows to the table
	function appendRows(){
		//create the tbody element
		var $tbody = $('<tbody></tbody>');
		
		//go thru all the rows in the cross-reference array
		rows.forEach(function(row){
			//add the HTML for the row
			$tbody.append(row.$element);
		});
		
		//add the body to the table
		$table.append($tbody);
	}
	
	//this function updates the table content, showing and hiding what is necessary
	function update(min, max){
		//go thru all the rows
		rows.forEach(function(row){
			//if the row is in the range...
			if(row.person.rate >= min && row.person.rate <= max){
				//...show the row
				row.$element.show();
			}
			else{
				//else we need to hide it
				row.$element.hide();
			}
		});
	}
	
	//this init function will set everything up for userAgent
	function init(){
		$('#slider').noUiSlider({
			range: [0, 150], start: [65, 90], handles: 2, margin: 20, connect: true,
			serialization: { to: [$min, $max], resolution: 1 }
		}).change(function(){ update($min.val(), $max.val()); });
		
		makeRows();
		appendRows();
		update($min.val(), $max.val());
	}
	
	//call init when DOM is ready
	$(init); 
}());









