/******************************************************************************
 * FILE NAME:	films.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Routes requests to the /films path, and performs mysql
 * 		queries for displaying, adding, updating, and deleting films.	
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
	// Select all attributes of all films and the ID, first name, and last name of their directors
	function getFilms(res, mysql, context, complete){
		mysql.pool.query("SELECT Films.ID, Films.directorID, Films.title, DATE_FORMAT(Films.releaseDate, '%Y-%m-%d') AS releaseDate, Films.openingWeekend, Directors.firstName, Directors.lastName FROM Films LEFT JOIN Directors ON Films.directorID=Directors.ID ORDER BY Films.title", function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.films = results;
			complete();
		});
	}
	
	// Select all attributes of all directors
	function getDirectors(res, mysql, context, complete){
		mysql.pool.query("SELECT ID, firstName, lastName FROM Directors", function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.directors = results;
			complete();
		});
	}
	
	// Select all attributes of a specific film
	function getFilm(res, mysql, context, ID, complete){
		var sql = "SELECT ID, title, DATE_FORMAT(releaseDate, '%Y-%m-%d') AS releaseDate, openingWeekend, directorID FROM Films WHERE ID = ?";
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
	
	/***** ROUTERS ******/

	// Display all films and the first name and last name of their directors
	router.get('/', function(req, res){
		var callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["selectedTable.js", "deleteFilm.js"];
		getFilms(res, mysql, context, complete);
		getTables(res, mysql, context, complete);
		getDirectors(res, mysql, context, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >= 3){
				res.render("films", context);
			}
		}
	});
	
	// Post request for adding a new film
	router.post('/', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "INSERT INTO Films (title, releaseDate, openingWeekend, directorID) VALUES (?,?,?,?)";
		var inserts = [req.body.title, req.body.releaseDate, req.body.openingWeekend, req.body.directorID];
		sql = mysql.pool.query(sql,inserts,function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			else{
				res.redirect('/films');
			}
		});
	});

	// Directs user to page for updating a film
	router.get('/:ID', function(req, res){
		callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["selectedDirector.js", "updateFilm.js"];
		getFilm(res, mysql, context, req.params.ID, complete);
		getDirectors(res, mysql, context, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >=2){
				res.render('updateFilm', context);
			}
		}
	});

	// Put request that performs update query 
	router.put('/:ID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "UPDATE Films SET title=?, releaseDate=?, openingWeekend=?, directorID=? WHERE ID=?";
		var inserts = [req.body.title, req.body.releaseDate, req.body.openingWeekend, req.body.directorID, req.params.ID];
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
	
	// delete request performs deletion query for specified film
	router.delete('/:ID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "DELETE FROM Films WHERE ID = ?";
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
