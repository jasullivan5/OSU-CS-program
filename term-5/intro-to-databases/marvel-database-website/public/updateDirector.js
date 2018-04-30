/******************************************************************************
 * FILE NAME:	updateDirector.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * description: performs ajax put request for updating specified director, 
 * 		then returns the user to the /directors page.
 *****************************************************************************/
function updateDirector(ID){
	$.ajax({
		url: '/directors/' + ID,
		type: 'PUT',
		data: $('#updateDirector').serialize(),
		success: function(result){
			window.location.replace("/directors");
		}
	})
};
