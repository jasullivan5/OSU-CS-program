const express = require('express');
const axios = require('axios');
const jwt = require('express-jwt');
const jwksRsa = require('jwks-rsa');
const model = require('./model');
const RECIPES = "Recipes";
const INGREDIENTS = "Ingredients";
const ORIGIN = "https://dev-cwqb4ghq.auth0.com/";
const CLIENT_ID = "H98NSNSv3cMRn3J17kr3CJ0eu3_zxkEl";
const CLIENT_SECRET = "NcxTvico7QtmMMFnyYRVqQuXKZmc7xs0IG7PTX1VxcFThJre91WWHmahZJikChgW";

const router = express.Router();

// Get user's list of recipes
router.get('/:user_id/recipes', async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    const result = await model.getOwnersEntities(RECIPES, req.params.user_id);
    res.status(200).json(result);
})

// Get user's list of ingredients
router.get('/:user_id/ingredients', async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    const result = await model.getOwnersEntities(INGREDIENTS, req.params.user_id);
    res.status(200).json(result);
})

// create user
router.post('/', async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    const tokenRequest = {
        grant_type: 'client_credentials',
        client_id: CLIENT_ID,
        client_secret: CLIENT_SECRET,
        audience: ORIGIN + 'api/v2/'
    };
    try{
        const tokenResponse = await axios.post(ORIGIN + 'oauth/token', tokenRequest);
        console.log(tokenResponse.data);
        const createUserRequest = {
            method: 'post',
            url: ORIGIN + 'api/v2/users',
            headers: {'Authorization': 'Bearer ' + tokenResponse.data.access_token},
            data: {
                "connection": "Username-Password-Authentication",
                "email": req.body.email,
                "password": req.body.password
            }
        };
        const response = await axios(createUserRequest);
        console.log(response.data);
        res.status(201).send(response.data);
    } catch(e){
        console.log(e);
        return res.status(403).end();
    }
});

//delete user
router.delete('/:user_id', async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    const tokenRequest = {
        grant_type: 'client_credentials',
        client_id: CLIENT_ID,
        client_secret: CLIENT_SECRET,
        audience: ORIGIN + 'api/v2/'
    };
    try{
        const tokenResponse = await axios.post(ORIGIN + 'oauth/token', tokenRequest);
        console.log(tokenResponse.data);
        const deleteUserRequest = {
            method: 'delete',
            url: ORIGIN + 'api/v2/users/' + req.params.user_id,
            headers: {'Authorization': 'Bearer ' + tokenResponse.data.access_token}
        };
        const response = await axios(deleteUserRequest);
        console.log(response.data);
        res.status(204).end();
    } catch(e){
        console.log(e);
        return res.status(403).end();
    }
})

//login
router.post('/login', async function(req, res){
    if(!req.accepts('application/json')){
        return res.status(406).end();
    }
    const body = {
        grant_type: 'password',
        username: req.body.username,
        password: req.body.password,
        client_id: CLIENT_ID,
        client_secret: CLIENT_SECRET
    };
    try{
        const response = await axios.post(ORIGIN + 'oauth/token', body);
        res.status(200).send(response.data);
    } catch(e){
        console.log(e);
        res.status(403).end();
    }
});

// All other HTTP verbs sent to root get rejected with code 405
router.all('/', function(req, res){
    res.status(405).end();
})

module.exports = router;
