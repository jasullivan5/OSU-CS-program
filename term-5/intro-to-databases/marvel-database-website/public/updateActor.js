/******************************************************************************
 * FILE NAME:	updateActor.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * description: performs ajax put request for updating specified actor,
 * 		then returns the user to the /actors page.
 *****************************************************************************/
function updateActor(ID){
	$.ajax({
		url: '/actors/' + ID,
		type: 'PUT',
		data: $('#updateActor').serialize(),
		success: function(result){
			window.location.replace("/actors");
		}
	})
};
