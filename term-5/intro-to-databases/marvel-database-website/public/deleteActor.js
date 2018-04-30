/******************************************************************************
 * FILE NAME:	deleteActor.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Sends ajax request for deletion of the specified actor, then
 * 		reloads the document.
 *****************************************************************************/
function deleteActor(ID){
	$.ajax({
		url: '/actors/' + ID,
		type: 'DELETE',
		success: function(result){
			window.location.reload(true);
		}
	})
};
