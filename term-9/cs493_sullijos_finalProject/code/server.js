const express = require('express'); // include for creating web app
const bodyParser = require('body-parser'); // include bodyParser for parsing request bodies
const users = require('./routes/users');
const recipes = require('./routes/recipes');
const ingredients = require('./routes/ingredients');
const PORT = process.env.PORT || 8080;

// Create web app
const app = express();

// Set body parser as middle where for all web app requests
app.use(bodyParser.json());

// Mount routes to app
app.use('/users', users);
app.use('/recipes', recipes);
app.use('/ingredients', ingredients);

// Listen on port for requests
app.listen(PORT, function(){
    console.log(`Server listening on port ${PORT}...`);
});
