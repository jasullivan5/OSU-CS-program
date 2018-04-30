/******************************************************************************
 * FILE NAME:	home.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Routes requests to the / path. Renders a page that allows
 * 		the user to select a table for viewing.	
 *****************************************************************************/

// Set up express
module.exports = function(){
	var express = require('express');
	var router = express.Router();

	// Select names of all tables	
	function getTables(res, mysql, context){
		mysql.pool.query("SELECT table_name FROM information_schema.tables WHERE table_schema='cs340_sullijos'", function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.tables = results;
			res.render('home', context);
		});
	}

	// Display page that allows user to navigate to the various table pages
	router.get('/', function(req, res){
		var callbackCount = 0;
		var context = {};
		context.jsScripts = ["selectedTable.js"];
		var mysql = req.app.get('mysql');
		getTables(res, mysql, context);
	})
	return router;
}();
