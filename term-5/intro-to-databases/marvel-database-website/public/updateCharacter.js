/******************************************************************************
 * FILE NAME:	updateCharacter.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * description: performs ajax put request for updating specified character,
 * 		then returns the user to the /characters page.
 *****************************************************************************/
function updateCharacter(ID){
	$.ajax({
		url: '/characters/' + ID,
		type: 'PUT',
		data: $('#updateCharacter').serialize(),
		success: function(result){
			window.location.replace("/characters");
		}
	})
};
