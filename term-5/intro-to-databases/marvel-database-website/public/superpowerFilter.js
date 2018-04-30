/******************************************************************************
 * FILE NAME:	superpowerFilter.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Directs the user to either a page that displays all characters
 * 		or a page that displays all characters that posess the superpower
 *		specified by the value argument passed in.
 *****************************************************************************/
function superpowerFilter(value){
	if (value == "all") {
		window.location.href = '/characters';
	}
	else {
		window.location.href =  '/characters/spfilter/' + value;
	}
};
