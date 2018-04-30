/******************************************************************************
 * FILE NAME:	updateFilm.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * description: performs ajax put request for updating specified film,
 * 		then returns the user to the /films page.
 *****************************************************************************/
function updateFilm(ID){
	$.ajax({
		url: '/films/' + ID,
		type: 'PUT',
		data: $('#updateFilm').serialize(),
		success: function(result){
			window.location.replace("/films");
		}
	})
};
