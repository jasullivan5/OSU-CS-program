const helper = require('./helper')
const {Datastore} = require('@google-cloud/datastore');
const datastore = new Datastore({projectId:'cs493-sullijos-finalproject'});
const BAD_PROPERTIES = "bad_properties";
const RECIPES = "Recipes";
const INGREDIENTS = "Ingredients";
const BAD_ID = "bad_id";
const NOT_OWNER = "not_owner";
const DELETED = "deleted";
const ADDED = "added";
const REMOVED = "removed";
const PAGE_LIMIT = 5;

async function getAllEntities(kind, cursor){
    console.log("getAllEntties()");
    var count = 0;
    const listKey = kind.toLowerCase();
    const countResult = await helper.getAllOfKind(kind);
    countResult[0].forEach(function(entity){
        count++;
        console.log(JSON.stringify(entity) + "\n");
    })
    const result = await helper.getAllOfKind(kind, cursor, PAGE_LIMIT);
    result[0].forEach(function(entity){
        helper.addID(entity);
        entity.self = helper.makeURL(kind, entity.id);
        helper.addListUrls(kind, entity);
    })

    var response = {
        "count": count,
    }
    response[listKey] = result[0];
    if(result[1].moreResults !== Datastore.NO_MORE_RESULTS){
        response.next = helper.nextURL(kind, result[1].endCursor);
    }
    return response;
}

async function getEntity(kind, id){
    console.log("getEntity()");
    var entity = await helper.getEntityByID(kind, id);
    if(!entity){
        return BAD_ID;
    }
    helper.addID(entity);
    entity.self = helper.makeURL(kind, entity.id);
    helper.addListUrls(kind, entity);
    return entity;
}

async function addEntity(kind, obj){
    console.log("addEntity()");
    if(!helper.validateEntity(kind, obj)){
        return BAD_PROPERTIES;
    }
    obj = helper.formatEntity(kind, obj);
    obj = helper.toDatastore(kind, obj);
    obj = await datastore.insert(obj);
    const id = obj[0].mutationResults[0].key.path[0].id;
    const url = helper.makeURL(kind, id);
    return url;
}

async function deleteEntity(kind, id, owner){
    console.log("deleteEntity()");
    var entity = await helper.getEntityByID(kind, id);
    if(!entity){
        return BAD_ID;
    }
    if(entity.owner !== owner){
        return NOT_OWNER;
    }
    await helper.removeFromLists(kind, id, entity);
    await datastore.delete(entity[Datastore.KEY]);
    return DELETED;
}

async function updateEntity(kind, id, obj, owner){
    console.log("updateEntity()");
    // Validate new non-list properties and that list property exists.
    if(!helper.validateEntity(kind, obj) || !helper.hasList(kind, obj)){
        return BAD_PROPERTIES;
    }
    // Get original entity from datastore
    var entity = await helper.getEntityByID(kind, id);
    // If no entity is found
    if(!entity){
        return BAD_ID;
    }
    // If owner doesn't match
    if(entity.owner !== owner){
        return NOT_OWNER;
    }
    // Validate new list property
    const result = await helper.validateList(kind, obj);
    if(!result){
        return BAD_ID;
    }
    helper.updateLists(result.kind, entity[result.key], result.list, id);
    obj = helper.formatEntity(kind, obj);
    obj[result.key] = result.list;
    obj = helper.toDatastore(kind, obj, id);
    obj = await datastore.update(obj);
    const url = helper.makeURL(kind, id);
    return url;
}

async function addIngredient(recipeID, ingredientID, owner){
    var recipe = await helper.getEntityByID(RECIPES, recipeID);
    if(!recipe){
        return BAD_ID;
    }
    if(recipe.owner !== owner){
        return NOT_OWNER;
    }
    var ingredient = await helper.getEntityByID(INGREDIENTS, ingredientID);
    if(!ingredient){
        return BAD_ID;
    }
    if(ingredient.owner !== owner){
        return NOT_OWNER;
    }
    recipe.ingredients.forEach(function(ingredient){
        if(ingredientID === ingredient.id){
            return BAD_ID;
        }
    })
    await helper.listAdd(RECIPES, recipeID, ingredientID);
    await helper.listAdd(INGREDIENTS, ingredientID, recipeID);
    return ADDED;
}

async function removeIngredient(recipeID, ingredientID, owner){
    var recipe = await helper.getEntityByID(RECIPES, recipeID);
    if(!recipe){
        return BAD_ID;
    }
    if(recipe.owner !== owner){
        return NOT_OWNER;
    }
    var ingredient = await helper.getEntityByID(INGREDIENTS, ingredientID);
    if(!ingredient){
        return BAD_ID;
    }
    if(ingredient.owner !== owner){
        return NOT_OWNER;
    }
    var found = false;
    for(var i = 0; i < recipe.ingredients.length; i++){
        if(ingredientID === recipe.ingredients[i].id){
            found = true;
            await helper.listRemove(RECIPES, recipeID, ingredientID);
            await helper.listRemove(INGREDIENTS, ingredientID, recipeID);
            return REMOVED;
        }
    }
    return BAD_ID;
}

async function getOwnersEntities(kind, owner){
    console.log("getOwnersEntities()");
    var result = await helper.queryOwnersEntities(kind, owner);
    result.forEach(function(entity){
        helper.addID(entity);
        entity.self = helper.makeURL(kind, entity.id);
        helper.addListUrls(kind, entity);
    })
    return result;
}

async function deleteAllEntities(){
    console.log("deleteAllEntities()");
    var allRecipes = await helper.queryAll(RECIPES);
    var keys = [];
    allRecipes.forEach(function(recipe){
        keys.push(recipe[Datastore.KEY]);
    })
    if(keys.length > 0){
        await datastore.delete(keys);
    }

    var allIngredients = await helper.queryAll(INGREDIENTS);
    keys = [];
    allIngredients.forEach(function(ingredient){
        keys.push(ingredient[Datastore.KEY]);
    })
    if(keys.length > 0){
        await datastore.delete(keys);
    }
}

module.exports = {
    getAllEntities,
    getEntity,
    addEntity,
    deleteEntity,
    updateEntity,
    addIngredient,
    removeIngredient,
    getOwnersEntities,
    deleteAllEntities
};
