#ifndef CARD_FUNC
#define CARD_FUNC

//parameters are gamestate state struct, int currentPlayer, int drawnTreasure, int hand counter, int [] tempHand, 
void adventurer_card(struct gameState *, int, int, int,  int[] );
//parameters are state struct, int currentPlayer, int handPos
void smithy_card(struct gameState *, int, int);
void village_card(struct gameState *, int, int);
//parameters are state struct, int currentPlayer, int card choice, int handPos
void salvager_card(struct gameState*, int, int, int);
//parameters are state struct, int currentPlayer, int handPos
int treasure_map_card(struct gameState*, int, int);
#endif