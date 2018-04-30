/******************************************************************************
 * FILE NAME:	has.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Routes requests to the /has path, and performs mysql
 * 		queries for displaying, adding, updating, and deleting has relationships.	
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

	// Select the character ID, superpower ID, first name, last name, alias, and superpower name for all has relationships
	function getHas(res, mysql, context, complete){
		mysql.pool.query("SELECT Has.superpowerID, Has.characterID, Characters.firstName, Characters.lastName, Characters.alias, Superpowers.name FROM Characters JOIN Has ON Characters.ID=Has.characterID JOIN Superpowers ON Has.superpowerID=Superpowers.ID ORDER BY Superpowers.name", function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.has = results;
			complete();
		});
	}
	
	// Select the ID, first name, last name, and alias of all characters
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
	
	// Select all attributes of all superpowers
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

	// Select the ID, first name, last name, and alias of a specified character
	function getCharacter(res, mysql, context, ID, complete){
		var sql = "SELECT ID, firstName, lastName, alias FROM Characters WHERE ID = ?";
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
	
	// Select all attributes of a specified superpower
	function getSuperpower(res, mysql, context, ID, complete){
		var sql = "SELECT ID, name FROM Superpowers WHERE ID = ?";
		var inserts = [ID];
		mysql.pool.query(sql, inserts, function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.superpower = results[0];
			complete();
		});
	}
	
	/***** ROUTERS *****/

	// Display all character has superpower relationships
	router.get('/', function(req, res){
		var callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["selectedTable.js", "deleteHas.js"];
		getHas(res, mysql, context, complete);
		getTables(res, mysql, context, complete);
		getCharacters(res, mysql, context, complete);
		getSuperpowers(res, mysql, context, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >= 4){
				res.render('has', context);
			}
		}
	});

	// Post request for adding a new has relationship 
	router.post('/', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "INSERT INTO Has (characterID, superpowerID) VALUES (?,?)";
		var inserts = [req.body.characterID, req.body.superpowerID];
		sql = mysql.pool.query(sql,inserts,function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			else{
				res.redirect('/has');
			}
		});
	});

	// Directs user to page for updating has relationship
	router.get('/:characterID/:superpowerID', function(req, res){
		callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["selectedCharacter.js", "selectedSuperpower.js", "updateHas.js"];
		getCharacter(res, mysql, context, req.params.characterID, complete);
		getCharacters(res, mysql, context, complete);
		getSuperpower(res, mysql, context, req.params.superpowerID, complete);
		getSuperpowers(res, mysql, context, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >=4){
				res.render('updateHas', context);
			}
		}
	});
	
	// Performs update query for has relationship
	router.put('/:characterID/:superpowerID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "UPDATE Has SET characterID=?, superpowerID=? WHERE characterID=? AND superpowerID=?";
		var inserts = [req.body.characterID, req.body.superpowerID, req.params.characterID, req.params.superpowerID];
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
	
	// delete request performs delete query of specified has relationship
	router.delete('/:characterID/:superpowerID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "DELETE FROM Has WHERE characterID=? AND superpowerID=?";
		var inserts = [req.params.characterID, req.params.superpowerID];
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
