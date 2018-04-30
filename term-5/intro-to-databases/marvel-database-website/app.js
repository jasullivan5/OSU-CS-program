/******************************************************************************
 * FILE NAME:	app.js
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 340
 * ASSIGNMENT:	Final Project
 * START DATE:	2/19/2018
 * DESCRIPTION:	app.js sets up express, mysql, body parser, and handelbars.
 * 		Then, it mounts the routers for each table in the DB to their
 * 		respective paths and the error routers to their paths.
 * 		Finally, designates ports to listen for requests on.
 *****************************************************************************/

// Set up express, mysql, body parser, and handlebars
var express = require('express');
var mysql = require('./dbcon.js');
var bodyParser = require('body-parser');
var app = express();
var handlebars = require('express-handlebars').create({defaultLayout:'main'});
app.engine('handlebars', handlebars.engine);
app.use(bodyParser.urlencoded({extended:true}));
app.set('view engine', 'handlebars');
app.set('mysql', mysql);

// designate path/directory for static files
app.use('/static', express.static('public'));

// set port from command line argument
app.set('port', process.argv[2]);

// Mount routers for each table to their respective paths
app.use('/', require('./home.js'));
app.use('/characters', require('./characters.js'));
app.use('/actors', require('./actors.js'));
app.use('/directors', require('./directors.js'));
app.use('/films', require('./films.js'));
app.use('/superpowers', require('./superpowers.js'));
app.use('/has', require('./has.js'));
app.use('/isin', require('./isin.js'));

// Mount error 404 page not found router
app.use(function(req, res){
	res.status(404);
	res.render('404');
});

// Mount error 500 router
app.use(function(err, req, res, next){
	console.error(err.stack);
	res.status(500);
	res.render('500');
});

// Designate port to listen for requests on and print message that epxress has been started
app.listen(app.get('port'), function(){
	console.log('Express started on http://flip3.engr.oregonstate.edu:' + app.get('port') + '; press Ctrl-C to terminate.');
});
