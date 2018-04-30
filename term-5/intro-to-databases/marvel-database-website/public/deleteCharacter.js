/******************************************************************************
 * FILE NAME:	deleteCharacter.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Sends ajax request for deletion of the specified character, then
 * 		reloads the document.
 *****************************************************************************/
function deleteCharacter(ID){
	$.ajax({
		url: '/characters/' + ID,
		type: 'DELETE',
		success: function(result){
			window.location.reload(true);
		}
	})
};
