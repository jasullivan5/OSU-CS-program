/******************************************************************************
 * FILE NAME:	updateHas.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * description: performs ajax put request for updating specified has relationship,
 * 		then returns the user to the /has page.
 *****************************************************************************/
function updateHas(characterID, superpowerID){
	$.ajax({
		url: '/has/' + characterID + '/' + superpowerID,
		type: 'PUT',
		data: $('#updateHas').serialize(),
		success: function(result){
			window.location.replace("/has");
		}
	})
};
