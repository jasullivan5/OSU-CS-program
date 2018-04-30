/******************************************************************************
 * FILE NAME:	selectedTable.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Directs the user to the table page of the table they select
 * 		from the tableSelect select form. Resets the select form
 * 		before changing locations.
 *****************************************************************************/
document.addEventListener("DOMContentLoaded", function(event){
	var tableSelect = document.getElementById("tableSelect");
	
	document.body.onbeforeunload = function(){
		tableSelect.selectedIndex = 0;
	};

	tableSelect.onchange = function(){
		window.location.href = this.value;
	};
});
