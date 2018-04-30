/******************************************************************************
 * FILE NAME:	updateIsIn.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * description: performs ajax put request for updating specified is in relationship,
 * 		then returns the user to the /isin page.
 *****************************************************************************/
function updateIsIn(characterID, filmID){
	$.ajax({
		url: '/isin/' + characterID + '/' + filmID,
		type: 'PUT',
		data: $('#updateIsIn').serialize(),
		success: function(result){
			window.location.replace("/isin");
		}
	})
};
