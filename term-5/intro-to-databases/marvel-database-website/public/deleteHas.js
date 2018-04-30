/******************************************************************************
 * FILE NAME:	deleteHas.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Sends ajax request for deletion of the specified
 *		has relationship, then reloads the document.
 *****************************************************************************/
function deleteHas(characterID, superpowerID){
	$.ajax({
		url: '/has/' + characterID + '/' + superpowerID,
		type: 'DELETE',
		success: function(result){
			window.location.reload(true);
		}
	})
};
