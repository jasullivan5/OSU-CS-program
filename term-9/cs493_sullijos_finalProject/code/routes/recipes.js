const express = require('express');
const model = require('./model');
const jwt = require('express-jwt');
const jwksRsa = require('jwks-rsa');
const ORIGIN = "https://dev-cwqb4ghq.auth0.com/";
const RECIPES = "Recipes";
const BAD_PROPERTIES = "bad_properties";
const BAD_ID = "bad_id";
const NOT_OWNER = "not_owner";

function setCode(result, code){
    switch (result) {
        case BAD_PROPERTIES:
            return 422;
        case BAD_ID:
            return 404;
        case NOT_OWNER:
            return 403;
    }
    return code;
}

const checkJwt = jwt({
    secret: jwksRsa.expressJwtSecret({
      cache: true,
      rateLimit: true,
      jwksRequestsPerMinute: 8,
      jwksUri: ORIGIN + '.well-known/jwks.json'
    }),
    // Validate the audience and the issuer.
    issuer: ORIGIN,
    algorithms: ['RS256']
});

const router = express.Router();

// Get all of kind
router.get('/', async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    var cursor = null;
    if(req.query.hasOwnProperty('cursor')){
        cursor = req.query.cursor;
    }
    const result = await model.getAllEntities(RECIPES, cursor);
    res.status(200).json(result);
})

// Get a single entity
router.get('/:id', async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    var result = await model.getEntity(RECIPES, req.params.id);
    const code = setCode(result, 200);
    if(code !== 200){
        return res.status(code).end();
    }
    res.status(code).send(result);
})

// Post recipe
router.post('/', checkJwt, async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    req.body.owner = req.user.name;
    const result = await model.addEntity(RECIPES, req.body);
    const code = setCode(result, 201);
    if(code === 201){
        res.set('Location', result);
    }
    res.status(code).end();
})

router.delete('/delete_all', async function(req, res){
    await model.deleteAllEntities();
    res.status(204).end();
})

router.delete('/:recipe_id/ingredients/:ingredient_id', checkJwt, async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    const result = await model.removeIngredient(req.params.recipe_id, req.params.ingredient_id, req.user.name)
    const code = setCode(result, 204);
    res.status(code).end();
})

// Delete entity
router.delete('/:id', checkJwt, async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    const result = await model.deleteEntity(RECIPES, req.params.id, req.user.name);
    const code = setCode(result, 204);
    res.status(code).end();
})

router.put('/:recipe_id/ingredients/:ingredient_id', checkJwt, async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    const result = await model.addIngredient(req.params.recipe_id, req.params.ingredient_id, req.user.name);
    const code = setCode(result, 204);
    res.status(code).end();
})

router.put('/:id', checkJwt, async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    req.body.owner = req.user.name;
    const result = await model.updateEntity(RECIPES, req.params.id, req.body, req.user.name);
    const code = setCode(result, 204);
    if(code === 204){
        res.set('Location', result);
    }
    res.status(code).end();
})

// All other HTTP verbs sent to root get rejected with code 405
router.all('/', function(req, res){
    res.status(405).end();
})

module.exports = router;
