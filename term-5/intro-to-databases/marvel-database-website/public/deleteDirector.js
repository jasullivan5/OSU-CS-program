/******************************************************************************
 * FILE NAME:	deleteDirector.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Sends ajax request for deletion of the specified director, then
 * 		reloads the document.
 *****************************************************************************/
function deleteDirector(ID){
	$.ajax({
		url: '/directors/' + ID,
		type: 'DELETE',
		success: function(result){
			window.location.reload(true);
		}
	})
};