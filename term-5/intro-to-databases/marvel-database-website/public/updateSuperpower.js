/******************************************************************************
 * FILE NAME:	updateSuperpower.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * description: performs ajax put request for updating specified superpower,
 * 		then returns the user to the /superpowers page.
 *****************************************************************************/
function updateSuperpower(ID){
	$.ajax({
		url: '/superpowers/' + ID,
		type: 'PUT',
		data: $('#updateSuperpower').serialize(),
		success: function(result){
			window.location.replace("/superpowers");
		}
	})
};
