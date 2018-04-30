/******************************************************************************
 * FILE NAME:	isin.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Routes requests to the /isin path, and performs mysql
 * 		queries for displaying, adding, updating, and deleting isin relationships.	
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

	// Select character ID, film ID, first name, last name, alias, and title for all is in relationships
	function getIsIn(res, mysql, context, complete){
		mysql.pool.query("SELECT IsIn.characterID, IsIn.filmID, Characters.firstName, Characters.lastName, Characters.alias, Films.title FROM Characters JOIN IsIn ON Characters.ID=IsIn.characterID JOIN Films ON IsIn.filmID=Films.ID ORDER BY Films.title", function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.isin = results;
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
	
	// Select the ID and title of all films
	function getFilms(res, mysql, context, complete){
		mysql.pool.query("SELECT ID, title FROM Films", function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.films = results;
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
	
	// Select the ID and title of a specific film 
	function getFilm(res, mysql, context, ID, complete){
		var sql = "SELECT ID, title FROM Films WHERE ID = ?";
		var inserts = [ID];
		mysql.pool.query(sql, inserts, function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.film = results[0];
			complete();
		});
	}

	/***** ROUTERS *****/

	// Display all character is in film relationships
	router.get('/', function(req, res){
		var callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["selectedTable.js", "deleteIsIn.js"];
		getIsIn(res, mysql, context, complete);
		getTables(res, mysql, context, complete);
		getFilms(res, mysql, context, complete);
		getCharacters(res, mysql, context, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >= 4){
				res.render('isin', context);
			}
		}
	});

	// Post request for adding a new is in relationship
	router.post('/', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "INSERT INTO IsIn (filmID, characterID) VALUES (?,?)";
		var inserts = [req.body.filmID, req.body.characterID];
		sql = mysql.pool.query(sql,inserts,function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			else{
				res.redirect('/isin');
			}
		});
	});	

	// Directs user to page for updating is in relationship	
	router.get('/:characterID/:filmID', function(req, res){
		callbackCount = 0;
		var context = {};
		context.jsScripts = ["selectedCharacter.js", "selectedFilm.js", "updateIsIn.js"];
		var mysql = req.app.get('mysql');
		getCharacter(res, mysql, context, req.params.characterID, complete);
		getCharacters(res, mysql, context, complete);
		getFilm(res, mysql, context, req.params.filmID, complete);
		getFilms(res, mysql, context, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >=4){
				res.render('updateIsIn', context);
			}
		}
	});
	
	// Performs update query for is in relationship
	router.put('/:characterID/:filmID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "UPDATE IsIn SET characterID=?, filmID=? WHERE characterID=? AND filmID=?";
		var inserts = [req.body.characterID, req.body.filmID, req.params.characterID, req.params.filmID];
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

	// delete request perfoms delete query on specified is in relationship
	router.delete('/:characterID/:filmID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "DELETE FROM IsIn WHERE characterID=? AND filmID=?";
		var inserts = [req.params.characterID, req.params.filmID];
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
