var express = require('express');

var app = express();
var handlebars = require('express-handlebars').create({defaultLayout:'main'});

app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');
app.set('port', 3878);

app.use(express.static(__dirname + '/views/public'));

app.get('/',function(req, res){
  res.render('home');
});

app.get('/about-me',function(req, res){
  res.render('about-me');
});

app.get('/resume',function(req, res){
  res.render('resume');
});

app.get('/contact',function(req, res){
  res.render('contact');
});

// For errors
app.use(function(req,res){
  res.status(404);
  res.render('404');
});
app.use(function(err, req, res, next){
  console.error(err.stack);
  res.type('plain/text');
  res.status(500);
  res.render('500');
});

app.listen(app.get('port'), function(){
  console.log('Express started on http://flip3.engr.oregonstate.edu:' + app.get('port') + '; press Ctrl-C to terminate.');
});
