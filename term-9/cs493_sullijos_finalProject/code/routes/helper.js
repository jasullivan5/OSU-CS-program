const moment = require('moment');
require("moment-duration-format");
const {Datastore} = require('@google-cloud/datastore');
const datastore = new Datastore({projectId:'cs493-sullijos-finalproject'});
const URL = "https://cs493-sullijos-finalproject.appspot.com";
const RECIPES = "Recipes";
const INGREDIENTS = "Ingredients";
const NON_WHITE_SPACE = /\S/;

async function getAllOfKind(kind, cursor, limit = null){
    console.log("getAllOfKind()");
    var query;
    if(limit === null){
        query = datastore.createQuery(kind)
    }
    else{
        query = datastore.createQuery(kind).limit(limit);
    }
    if (cursor) {
        query = query.start(cursor);
    }
    return await datastore.runQuery(query);
}

async function queryAll(kind){
    console.log("queryAll()");
    const query = datastore.createQuery(kind);
    const result = await datastore.runQuery(query);
    return result[0];
}

async function getEntityByID(kind, id){
    console.log("getEntityByID()");
    if(!Number.isInteger(Number(id))){
        return null;
    }
    const key = datastore.key([kind, datastore.int(id)]);
    const result = await datastore.get(key);
    return result[0];
}

async function queryOwnersEntities(kind, owner){
    console.log("queryOwnersEntities()");
    const query = datastore.createQuery(kind).filter("owner", owner);
    const result = await datastore.runQuery(query);
    return result[0];
}

function addID(entity){
    console.log("addID()");
    entity.id = entity[Datastore.KEY].id;
}

function makeURL(kind, id){
    console.log("makeURL()");
    const url = URL + "/" + kind.toLowerCase() + "/" + id;
    return url;
}

function nextURL(kind, cursor){
    console.log("nextURL()");
    return URL + "/" + kind.toLowerCase() + "?cursor=" + cursor;
}

function addListUrls(kind, entity){
    if(kind === RECIPES){
        entity.ingredients.forEach(function(ingredient){
            ingredient.self = makeURL(INGREDIENTS, ingredient.id);
        });
    }
    if(kind === INGREDIENTS){
        entity.recipes.forEach(function(recipe){
            recipe.self = makeURL(RECIPES, recipe.id);
        });
    }
}

function validateEntity(kind, obj){
    if(kind === RECIPES){
        return validateRecipe(obj);
    }
    if(kind === INGREDIENTS){
        return validateIngredient(obj);
    }
}

function validateRecipe(recipe){
    console.log("validateRecipe()");
    const propKeys = Object.keys(recipe);
    return(
        propKeys.includes("name") &&
        validateStringProperty(recipe.name) &&
        propKeys.includes("prep_time") &&
        validateDurationProperty(recipe.prep_time) &&
        propKeys.includes("servings") &&
        validateNumberProperty(recipe.servings) &&
        propKeys.includes("owner") &&
        validateStringProperty(recipe.owner)
    );
}

function validateIngredient(ingredient){
    console.log("validateIngredient()");
    const propKeys = Object.keys(ingredient);
    return(
        propKeys.includes("name") &&
        validateStringProperty(ingredient.name) &&
        propKeys.includes("units") &&
        validateStringProperty(ingredient.units) &&
        propKeys.includes("quantity") &&
        validateNumberProperty(ingredient.quantity) &&
        propKeys.includes("owner") &&
        validateStringProperty(ingredient.owner)
    );
}

function validateStringProperty(property){
    return (typeof property === "string" && property.match(NON_WHITE_SPACE));
}

function validateNumberProperty(property){
    return (typeof property === "number" && property > 0);
}

function validateDurationProperty(property){
    return (moment.duration(property, "HH:mm", true).isValid());
}

function formatEntity(kind, obj){
    if(kind === RECIPES){
        return formatRecipe(obj);
    }
    if(kind === INGREDIENTS){
        return formatIngredient(obj);
    }
}

function formatRecipe(recipe){
    console.log("formatRecipe()");
    return {
        "name": recipe.name,
        "prep_time": recipe.prep_time,
        "servings": recipe.servings,
        "owner": recipe.owner,
        "ingredients": []
    }
}

function formatIngredient(ingredient){
    console.log("formatIngredient()");
    return {
        "name": ingredient.name,
        "units": ingredient.units,
        "quantity": ingredient.quantity,
        "owner": ingredient.owner,
        "recipes": []
    }
}

function hasList(kind, obj){
    if(kind === RECIPES && obj.hasOwnProperty('ingredients')){
        if(Array.isArray(obj.ingredients)){
            return true;
        }
    }
    if(kind === INGREDIENTS && obj.hasOwnProperty('recipes')){
        if(Array.isArray(obj.recipes)){
            return true;
        }
    }
    return false;
}

async function validateList(kind, obj){
    var result = getList(kind, obj);
    var counts = [];
    for(var i = 0; i < result.list.length; i++){
        if(!await getEntityByID(result.kind, result.list[i].id)){
            return null;
        }
        if(counts[result.list[i].id] === undefined){
            counts[result.list[i].id] = 1;
        }
        else{
            return null;
        }
    }
    return {"key": result.kind.toLowerCase(), "list": result.list, "kind": result.kind};
}

async function removeFromLists(kind, id, entity){
    const result = getList(kind, entity);
    result.list.forEach(async function(item){
        await listRemove(result.kind, item.id, id);
    });
}

async function updateLists(listKind, oldList, newList, entityID){
    var missing;
    var isNew;
    for(var i = 0; i < oldList.length; i++){
        missing = true;
        for(var j = 0; j < newList.length; j++){
            if(oldList[i].id === newList[j]){
                missing = false;
                break;
            }
        }
        if(missing){
            await listRemove(listKind, oldList[i].id, entityID);
        }
    }

    for (i = 0; i < newList.length; i++){
        isNew = true;
        for(j = 0; j < oldList.length; j++){
            if(newList[i].id === oldList[j].id){
                isNew = false;
                break;
            }
        }
        if(isNew){
            await listAdd(listKind, newList[i].id, entityID);
        }
    }
}

async function listRemove(kind, id, idToRemove){
    var entity = await getEntityByID(kind, id);
    var result = getList(kind, entity);
    for(var i = 0; i < result.list.length; i++){
        if(result.list[i].id === idToRemove){
            result.list.splice(i, 1);
            entity = toDatastore(kind, entity, id);
            return await datastore.update(entity);
        }
    }
    return null;
}

async function listAdd(kind, id, idToAdd){
    console.log("id to add:" + idToAdd);
    var entity = await getEntityByID(kind, id);
    var result = getList(kind, entity);
    for(var i = 0; i < result.list.length; i++){
        if(result.list[i].id === idToAdd){
            return null;
        }
    }
    result.list.push({"id": idToAdd});
    console.log("new list:" + JSON.stringify(result.list));
    console.log("entity:" + JSON.stringify(entity));
    entity = toDatastore(kind, entity, id);
    return await datastore.update(entity);
}

function getList(kind, entity){
    var listKind;
    var list;
    if(kind === RECIPES){
        list = entity.ingredients;
        listKind = INGREDIENTS;
    }
    if(kind === INGREDIENTS){
        list = entity.recipes;
        listKind = RECIPES;
    }
    return {"kind": listKind, "list": list};
}

function toDatastore(kind, obj, id = null){
    console.log("toDatastore()");
    var key;
    if(!id){
        key = datastore.key(kind);
    }
    else{
        key = datastore.key([kind, datastore.int(id)]);
    }
    const entity = {
        "key": key,
        "data": obj
    }
    return entity;
}

module.exports = {
    getAllOfKind,
    queryAll,
    getEntityByID,
    queryOwnersEntities,
    addID,
    makeURL,
    nextURL,
    addListUrls,
    validateEntity,
    hasList,
    validateList,
    listAdd,
    listRemove,
    updateLists,
    formatEntity,
    removeFromLists,
    toDatastore
}
