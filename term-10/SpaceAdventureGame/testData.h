
Player player(50,100); 

Item jacket("Jacket", "descriptorFiles\\jacket.txt", true); 
Item pickaxe("Pickaxe", "descriptorFiles\\pickaxe.txt", true); 

Area earth, mars, northAmerica, antarctica; 

earth.setLongDescription("This is Earth duh"); 
earth.setShortDescription("Still Earth"); 
mars.setShortDescription("Red planet"); 
mars.setLongDescription("The Red planet, Mars"); 

player.setLocation(&earth); 
earth.addItem(&jacket); 
earth.addItem(&pickaxe); 

Exit marsExit("Mars", "The way to mars is to launch on a space ship", &mars); 
earth.addExit(&marsExit); 

Exit northAmericaExit("NorthAmerica", "Head down to land on Mexico, USA, and Cananada", &northAmerica); 
earth.addExit(&northAmericaExit); 

Exit antarcticaExit("Antarctica", "Head south up to the penguins", &antarctica); 
northAmerica.addExit(&antarcticaExit); 
