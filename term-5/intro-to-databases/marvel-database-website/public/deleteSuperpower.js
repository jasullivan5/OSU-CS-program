/******************************************************************************
 * FILE NAME:	deleteSuperpower.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Sends ajax request for deletion of the specified
 *		superpower, then reloads the document.
 *****************************************************************************/
function deleteSuperpower(ID){
	$.ajax({
		url: '/superpowers/' + ID,
		type: 'DELETE',
		success: function(result){
			window.location.reload(true);
		}
	})
};
