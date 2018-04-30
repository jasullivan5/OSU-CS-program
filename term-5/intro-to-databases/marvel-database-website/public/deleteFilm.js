/******************************************************************************
 * FILE NAME:	deleteFilm.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Sends ajax request for deletion of the specified film, then
 * 		reloads the document.
 *****************************************************************************/
function deleteFilm(ID){
	$.ajax({
		url: '/films/' + ID,
		type: 'DELETE',
		success: function(result){
			window.location.reload(true);
		}
	})
};
