//adding items to start and end of list
var list = document.getElementsByTagName('ul')[0];

//add new item to end of list
var newItemLast = document.createElement('li');
var newTextLast = document.createTextNode('cream');
newItemLast.appendChild(newTextLast);
list.appendChild(newItemLast);

//add new element to start of list
var newItemFirst = document.createElement('li');
var newTextFirst = document.createTextNode('kale');
newItemFirst.appendChild(newTextFirst);
list.insertBefore(newItemFirst, list.firstChild);

//get all li elments
var listItems = document.querySelectorAll('li');

//add a class of cool to all list items
for(var i = 0; i < listItems.length; i++){
	listItems[i].className = 'cool';
}

//add number of items in the list to the heading
var heading = document.querySelector('h2');
var headingText = heading.firstChild.nodeValue;
var totalItems = listItems.length;
var newHeading = headingText + '<span>' + totalItems + '</span>';
heading.innerHTML = newHeading;