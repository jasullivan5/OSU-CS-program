/******************************************************************************
 * FILE NAME:	superpowers.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION: Routes requests to the /superpowers path, and performs mysql
 * 		queries for displaying, adding, updating, and deleting superpowers.	
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

	// Select all attributes for all superpowers 
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
	
	// Select all attributes of a specific superpower
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
	
	/***** ROUTERS ******/
	
	// Display all superpowers
	router.get('/', function(req, res){
		var callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["selectedTable.js", "deleteSuperpower.js"];
		getSuperpowers(res, mysql, context, complete);
		getTables(res, mysql, context, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >= 2){
				res.render('superpowers', context);
			}
		}
	});

	// Post request for adding a new superpower
	router.post('/', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "INSERT INTO Superpowers (name) VALUES (?)";
		var inserts = [req.body.name];
		sql = mysql.pool.query(sql,inserts,function(error, results, fields){
			if(error){
				res.write(JSON.stringify(error));
				res.end();
			}
			else{
				res.redirect('/superpowers');
			}
		});
	});

	// Directs user to page for updating superpower
	router.get('/:ID', function(req, res){
		callbackCount = 0;
		var context = {};
		var mysql = req.app.get('mysql');
		context.jsScripts = ["updateSuperpower.js"];
		getSuperpower(res, mysql, context, req.params.ID, complete);
		function complete(){
			callbackCount++;
			if(callbackCount >= 1){
				res.render('updateSuperpower', context);
			}
		}
	});

	// Put request that performs update query
	router.put('/:ID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "UPDATE Superpowers SET name=? WHERE ID=?";
		var inserts = [req.body.name, req.params.ID];
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
	
	// delete request performs deletion query for specified superpower
	router.delete('/:ID', function(req, res){
		var mysql = req.app.get('mysql');
		var sql = "DELETE FROM Superpowers WHERE ID = ?";
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
