const express = require('express');
const model = require('./model');
const jwt = require('express-jwt');
const jwksRsa = require('jwks-rsa');
const ORIGIN = "https://dev-cwqb4ghq.auth0.com/";
const INGREDIENTS = "Ingredients";
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
    const result = await model.getAllEntities(INGREDIENTS, cursor);
    res.status(200).json(result);
})

// Get a single entity
router.get('/:id', async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    var result = await model.getEntity(INGREDIENTS, req.params.id);
    const code = setCode(result, 200);
    if(code !== 200){
        return res.status(code).end();
    }
    res.status(code).send(result);
})

// Post ingredient
router.post('/', checkJwt, async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    req.body.owner = req.user.name;
    const result = await model.addEntity(INGREDIENTS, req.body);
    const code = setCode(result, 201);
    if(code === 201){
        res.set('Location', result);
    }
    res.status(code).end();
})

// Delete entity
router.delete('/:id', checkJwt, async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    const result = await model.deleteEntity(INGREDIENTS, req.params.id, req.user.name);
    const code = setCode(result, 204);
    res.status(code).end();
})

router.put('/:id', checkJwt, async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    req.body.owner = req.user.name;
    const result = await model.updateEntity(INGREDIENTS, req.params.id, req.body, req.user.name);
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
