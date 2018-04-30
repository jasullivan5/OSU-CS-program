/******************************************************************************
 * FILE NAME:	actors.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Routes requests to the /actors path, and performs mysql
 * 		queries for displaying, adding, updating, and deleting actors.	
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

	// Select all attributes of all actors and the ID, first name, last name, and alias of characters they play
	function getActors(res, mysql, context, complete){
		mysql.pool.query("SELECT Actors.ID, Actors.characterID, Actors.firstName, Actors.lastName, Characters.firstName AS cFirstName, Characters.lastName AS cLastName, Characters.alias FROM Actors LEFT JOIN Characters ON Actors.characterID=Characters.ID ORDER BY Characters.firstName", function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.actors = results;
			complete();
		});
	}

	// Select the ID, first name, last name, and alias all characters
	function getCharacters(res, mysql, context, complete){
		mysql.pool.query("SELECT ID, firstName, lastName, alias FROM Characters", function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.characters = results;
			complete();
		});
	}

	// Select all attributes of a specific actor
	function getActor(res, mysql, context, ID, complete){
		var sql = "SELECT ID, firstName, lastName, characterID FROM Actors WHERE ID = ?";
		var inserts = [ID];
		mysql.pool.query(sql, inserts, function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.actor = results[0];
			complete();
		});
	}
		
	/***** ROUTERS ******/

	// Display all actors and the first name, last name, and alias of characters they play
	router.get('/', function(req, res){
		var callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["selectedTable.js", "deleteActor.js"];
		getActors(res, mysql, context, complete);
		getTables(res, mysql, context, complete);
		getCharacters(res, mysql, context, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >= 3){
				res.render("actors", context);
			}
		}
	});

	// Post request for adding a new actor
	router.post('/', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "INSERT INTO Actors (firstName, lastName, characterID) VALUES (?,?,?)";
		var inserts = [req.body.firstName, req.body.lastName, req.body.characterID];
		sql = mysql.pool.query(sql,inserts,function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			else{
				res.redirect('/actors');
			}
		});
	});

	// Directs user to page for updating an actor
	router.get('/:ID', function(req, res){
		callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["selectedCharacter.js", "updateActor.js"];
		getActor(res, mysql, context, req.params.ID, complete);
		getCharacters(res, mysql, context, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >=2){
				res.render('updateActor', context);
			}
		}
	});

	// Put request that performs update query
	router.put('/:ID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "UPDATE Actors SET firstName=?, lastName=?, characterID=? WHERE ID=?";
		var inserts = [req.body.firstName, req.body.lastName, req.body.characterID, req.params.ID];
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
    
	// delete request performs deletion query for specified actor
	router.delete('/:ID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "DELETE FROM Actors WHERE ID = ?";
		var inserts = [req.params.ID];
		sql = mysql.pool.query(sql, inserts, function(error, results, fields){
		    if(error){
			res.write(JSON.stringify(error));
			res.status(400);
			res.end();
		    }else{
			res.status(202).end();
		    }
		})
	    })

	
	return router;
}();
