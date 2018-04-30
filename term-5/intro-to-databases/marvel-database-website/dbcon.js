/******************************************************************************
 * FILE NAME:	dbcon.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION:	Establishes connection with database.
 *****************************************************************************/
var mysql = require('mysql');
var pool = mysql.createPool({
	connectionLimit : 10,
	host		: 'classmysql.engr.oregonstate.edu',
    	user		: 'cs340_sullijos',
    	password	: '8637',
    	database	: 'cs340_sullijos'
});
module.exports.pool = pool;
