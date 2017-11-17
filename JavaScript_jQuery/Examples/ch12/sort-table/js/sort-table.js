$(function(){

	//define compare object
	var compare = {
		//the name function compares two names, taking out the beginning 'the'
		name: function(a, b){
			//remove '(T)the' form the start of a and b
			a = a.replace(/^the /i, '');
			b = b.replace(/^the /i, '');		
			
			//if a is less than b, return -1
			if(a < b){
				return -1;
			}
			//else return 0 if equal and 1 if a is greater than b
			else{
				return a > b ? 1 : 0;
			}
		},
		//the duration function compares durations
		duration: function(a, b){
			//split the time at the colon (here a and b are arrays)
			a = a.split(':');
			b = b.split(':');
			
			//convert the time to total seconds (now a and b are numbers)
			a = Number(a[0]) * 60 + Number(a[1]);
			b = Number(b[0]) * 60 + Number(b[1]);
			
			return a - b;
		},
		//the date function compares two dates
		date: function(a, b){
			//convert both a and b to dates
			a = new Date(a);
			b = new Date(b);
			
			return a - b;
		}
	};

	//get each sortable table
	$('.sortable').each(function(){
		//store the table
		var $table = $(this);
		
		//store the table body
		var $tbody = $table.find('tbody');
		
		//store the table headers
		var $controls = $table.find('th');
		
		//store array containing rows
		var rows = $tbody.find('tr').toArray();
		
		//event for when user clicks on headers
		$controls.on('click', function(){
			//get the header you clicked on
			var $header = $(this);
			
			//get value of data-sort attribute
			var order = $header.data('sort');
			
			//declare variable to hold index of clicked header
			var column;
			
			//if we've already selected this header, we don't need to run any functions, just reverse the order of the rows
			if($header.is('.ascending') || $header.is('.descending')){
				//toggle to the other class
				$header.toggleClass('ascending descending');
				
				//reverse the array
				$tbody.append(rows.reverse());
			}
			//else we need to perform a sort
			else{
				//start with ascending order
				$header.addClass('ascending');
				
				//remove ascending or descending from all other siblings
				$header.siblings().removeClass('ascending descending');
				
				//if the compare object has the method from the column
				if(compare.hasOwnProperty(order)){
					//search for the column's index number
					column = $controls.index(this);
					
					//sort the rows using by the column we want using the proper function for that column
					rows.sort(function(a, b){
						//get the text values from the proper column for the sort
						a = $(a).find('td').eq(column).text();
						b = $(b).find('td').eq(column).text();
						return compare[order](a, b);
					});
					
					//after teh sort, append the rows to the table
					$tbody.append(rows);
				}
			}
		});
	});

});










