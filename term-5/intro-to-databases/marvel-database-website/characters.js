/******************************************************************************
 * FILE NAME:	characters.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Routes requests to the /characters path, and performs mysql
 * 		queries for displaying, adding, updating, and deleting characters.	
 *****************************************************************************/

// Set up express
module.exports = function(){
	var express = require('express');
	var router = express.Router();

	/***** MYSQL SELECT QUERY FUNCTIONS *****/
	
	// Select names of all tables
	function getTables(res, mysql, context, complete){
		mysql.pool.query("SELECT table_name FROM information_schema.tables WHERE table_schema='cs340_sullijos'", function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.tables = results;
			complete();
		});
	}

	// Select all attributes of all characters
	function getCharacters(res, mysql, context, complete){
		mysql.pool.query("SELECT ID, firstName, lastName, alias, species FROM Characters", function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.characters = results;
			complete();
		});
	}

	// Select all attributes of characters with a specific superpower
	function getCharactersSpFilter(res, mysql, context, ID, complete){
		var sql = "SELECT ID, firstName, lastName, alias, species FROM Characters LEFT JOIN Has ON Characters.ID=Has.characterID WHERE Has.superpowerID=?";
		var inserts = [ID];
		mysql.pool.query(sql, inserts, function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.characters = results;
			complete();
		});
	}

	// Select all attributes of a single character
	function getCharacter(res, mysql, context, ID, complete){
		var sql = "SELECT ID, firstName, lastName, alias, species FROM Characters WHERE ID = ?";
		var inserts = [ID];
		mysql.pool.query(sql, inserts, function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.character = results[0];
			complete();
		});
	}
	
	// Select ID and name of all superpowers
	function getSuperpowers(res, mysql, context, complete){
		mysql.pool.query("SELECT ID, name FROM Superpowers", function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.superpowers = results;
			complete();
		});
	}
	
	/***** ROUTERS ******/

	// Display all characters
	router.get('/', function(req, res){
		// declare variables
		var callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		// Scripts for HTML header
		context.jsScripts = ["selectedTable.js", "deleteCharacter.js", "superpowerFilter.js", "selectedSuperpower.js"];
		// Selection for superpower filter
		context.selection = "all";
		// Queries to run before rendering page
		getCharacters(res, mysql, context, complete);
		getTables(res, mysql, context, complete);
		getSuperpowers(res, mysql, context, complete);
		// Function makes sure all query functions have been run before page is rendered
		function complete(){
			callbackCount++;
			if(callbackCount >= 3){
				res.render('characters', context);
			}
		}
	});
	
	// Display all characters who have the chosen superpower 
	router.get('/spfilter/:ID', function(req, res){
		var callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["selectedTable.js", "deleteCharacter.js", "superpowerFilter.js", "selectedSuperpower.js"];
		context.selection = req.params.ID;
		getCharactersSpFilter(res, mysql, context, req.params.ID, complete);
		getTables(res, mysql, context, complete);
		getSuperpowers(res, mysql, context, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >= 3){
				res.render('characters', context);
			}
		}
	});

	// Post request for adding a new character
	router.post('/', function(req, res){
		var mysql = req.app.get('mysql');
		// insert new character query
		var sql = "INSERT INTO Characters (firstName, lastName, alias, species) VALUES (?,?,?,?)";
		var inserts = [req.body.firstName, req.body.lastName, req.body.alias, req.body.species];
		sql = mysql.pool.query(sql,inserts,function(error, results, fields){
			// error handler
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			// redirect to /characters upon success
			else{
				res.redirect('/characters');
			}
		});
	});

	// Directs user to page for updating a character
	router.get('/:ID', function(req, res){
		callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["updateCharacter.js"];
		getCharacter(res, mysql, context, req.params.ID, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >= 1){
				res.render('updateCharacter', context);
			}
		}
	});

	// Put request that performs update query
	router.put('/:ID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "UPDATE Characters SET firstName=?, lastName=?, alias=?, species=? WHERE ID=?";
		var inserts = [req.body.firstName, req.body.lastName, req.body.alias, req.body.species, req.params.ID];
		sql = mysql.pool.query(sql,inserts,function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			else{
				res.status(200);
				res.end();
			}
		});
	});

	// delete request performs deletion query for specified character
	router.delete('/:ID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "DELETE FROM Characters WHERE ID = ?";
		var inserts = [req.params.ID];
		sql = mysql.pool.query(sql, inserts, function(error, results, fields){
		    if(error){
			res.write(JSON.stringify(error));
			res.status(400);
			res.end();
		    }
		    else{
			res.status(202).end();
		    }
		})
	    })

	return router;
}();
