/******************************************************************************
 * FILE NAME:	directors.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Routes requests to the /directors path, and performs mysql
 * 		queries for displaying, adding, updating, and deleting directors.
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
	
	// Select all attributes of a specified director
	function getDirector(res, mysql, context, ID, complete){
		var sql = "SELECT ID, firstName, lastName FROM Directors WHERE ID = ?";
		var inserts = [ID];
		mysql.pool.query(sql, inserts, function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			context.director = results[0];
			complete();
		});
	}

	/***** ROUTERS ******/
	
	// Display all directors
	router.get('/', function(req, res){
		var callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["selectedTable.js", "deleteDirector.js"];
		getDirectors(res, mysql, context, complete);
		getTables(res, mysql, context, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >= 2){
				res.render('directors', context);
			}
		}
	});

	// Post request for adding a new director
	router.post('/', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "INSERT INTO Directors (firstName, lastName) VALUES (?,?)";
		var inserts = [req.body.firstName, req.body.lastName];
		sql = mysql.pool.query(sql,inserts,function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			else{
				res.redirect('/directors');
			}
		});
	});

	// Directs user to page for updating a director
	router.get('/:ID', function(req, res){
		callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["updateDirector.js"];
		getDirector(res, mysql, context, req.params.ID, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >= 1){
				res.render('updateDirector', context);
			}
		}
	});

	// Put request that performs update query
	router.put('/:ID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "UPDATE Directors SET firstName=?, lastName=? WHERE ID=?";
		var inserts = [req.body.firstName, req.body.lastName, req.params.ID];
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
	
	// delete request performs deletion query for specified director
	router.delete('/:ID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "DELETE FROM Directors WHERE ID = ?";
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
