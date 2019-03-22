#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "card_functions.h"
/*
THESE FUNCTIONS ARE THE VERSION WITHOUT BUGS.  THOSE PLACED IN DOMINION.C ARE THE BUGGED ONES.

*/
//parameters are gamestate state struct, int currentPlayer, int drawnTreasure, int [] tempHand, 
void adventurer_card(struct gameState * state, int currentPlayer, int drawnTreasure, int temp_counter,  int temp_hand[])
{
    int cardDrawn; //id of card based on CARD ENUM in dominion.h
    while(drawnTreasure < 2)
    {
        if (state->deckCount[currentPlayer] < 1)
        { //if the deck is empty we need to shuffle discard and add to deck
            shuffle(currentPlayer, state);
        }
        drawCard(currentPlayer, state);
        cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer] - 1]; //top card of hand is most recently drawn card.
        if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)    //if card is a treasure card, increment drawnTreasure
            drawnTreasure++;
        else
        {
            temp_hand[temp_counter] = cardDrawn;
            state->handCount[currentPlayer]--; //this should just remove the top card (the most recently drawn one).
            temp_counter++;
        }
    }
    while (temp_counter - 1 >= 0)
    {
        state->discard[currentPlayer][state->discardCount[currentPlayer]++] = temp_hand[temp_counter - 1]; // discard all cards in play that have been drawn
        temp_counter = temp_counter - 1;
    }
}

void smithy_card(struct gameState * state, int currentPlayer, int handPos)
{
    //draw 3 cards
    for(int i = 0; i < 3; i++)
    {
        drawCard(currentPlayer, state);
    }
    //discard card
    discardCard(handPos, currentPlayer, state, 0);

}

void village_card(struct gameState * state, int currentPlayer, int handPos)
{
    //Draw card
    drawCard(currentPlayer, state);
    //Add 2 actions
    state->numActions = state->numActions + 2;
    //Discard card after effects are finished
    discardCard(handPos, currentPlayer, state, 0);

}

//parameters are state struct, int currentPlayer, int card choice, int handPos
void salvager_card(struct gameState* state , int currentPlayer, int choice, int handPos)
{
    //buys +1
    state->numBuys++;
    if(choice)
    {
        //gain coins = to value of card to be discarded
        state->coins = state->coins + getCost(handCard(choice, state));
        //discard card into destroy pile
        discardCard(choice, currentPlayer, state, 1);
    }
    //discard the salvager card into recycle pile
    discardCard(handPos, currentPlayer, state, 0 );

}

//parameters are state struct, int currentPlayer, int handPos
int treasure_map_card(struct gameState * state, int currentPlayer, int handPos)
{
    int index = -1;
    for(int i = 0; i < state->handCount[currentPlayer]; i++)
    {
        //if the i'th card is a treasure map that isn't the initial treasure map...
        if (state->hand[currentPlayer][i] == treasure_map && i != handPos)
        {
            index = i;
            break;
        }

    }
    if(index > -1) //if a treasure map was found....
    {
        //discard both into the trash pile
        discardCard(handPos, currentPlayer, state, 1);
        discardCard(index, currentPlayer, state, 1);
        //gain 4 gold cards
        for(int i = 0 ; i < 4; i++)
        {
            gainCard(gold, state, 1, currentPlayer);
        }
        return 1;
    }
    //else no 2nd treasure map found
    return -1;

}