/******************************************************************************
 * FILE NAME:	deleteIsIn.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Sends ajax request for deletion of the specified
 *		is in relationship, then reloads the document.
 *****************************************************************************/
function deleteIsIn(characterID, filmID){
	$.ajax({
		url: '/isin/' + characterID + '/' + filmID,
		type: 'DELETE',
		success: function(result){
			window.location.reload(true);
		}
	})
};
