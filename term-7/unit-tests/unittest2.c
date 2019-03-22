/*********************************************************************
** Program name:    testHandCard
** Author:          Joseph Sullivan, sullijos
** Date:            10/22/2018
** Description:     Unit test for function
**                  int handCard(int handPos, struct gameState *state) {
**                      int currentPlayer = whoseTurn(state);
**                      return state->hand[currentPlayer][handPos];
**                  }
**                  Tests function for boundary values of card, player number,
**                  and hand sizes.
*********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

char* getCardName(int card);
int myAssert(int expression, int* passedAllTests, char message[]);
void testStates(struct gameState* state, struct gameState* testState, int* passedAllTests);

int main(){
    int passedAllTests = 1;
    int kingdomCards[10] = {adventurer, council_room, feast, gardens, mine
                            , remodel, smithy, village, baron, great_hall};
    int seed = 1;
    struct gameState state, testState;
    int cardArray[MAX_HAND];

    // Initialize game
    memset(&state, 0, sizeof(struct gameState));
    initializeGame(MAX_PLAYERS, kingdomCards, seed, &state);

    // Begin test
    printf("TESTING handCard():\n");

    int i;
    for (i = curse; i <= treasure_map; i += treasure_map){ // testing card boundary values
        int j;
        for (j = 0; j < MAX_HAND; j++){ // create array of current card
            cardArray[j] = i;
        }
        for (j = 0; j < MAX_PLAYERS; j += (MAX_PLAYERS - 1)){ // for boundary player numbers
            state.whoseTurn = j;
            int k;
            for (k = 1; k <= MAX_HAND; k += (MAX_HAND - 1)){ // for boundary hand sizes
                // Fill hand with current card
                state.handCount[j] = k;
                memcpy(state.hand[j], cardArray, sizeof(int) * k);
                printf("Hand size = %d\n", k);
                int l;
                for (l = 0; l < k; l += (MAX_HAND - 1)){ // for boundary hand positions

                    // Copy game state for comparison after test function is called
                    memset(&testState, 0, sizeof(struct gameState));
                    memcpy(&testState, &state, sizeof(struct gameState));

                    // Test that card matches what was set
                    printf("Expecting: Player %d, hand position %d, card %s\n", j + 1, l, getCardName(i));
                    printf("   Result: Player %d, hand position %d, card %s\n", whoseTurn(&state) + 1, l, getCardName(handCard(l, &state)));
                    myAssert(handCard(l, &state) == i, &passedAllTests, "Result of handCard() does not match what was set.\n");

                    // Test game states
                    testStates(&state, &testState, &passedAllTests);
                }
            }
        }
    }
    // Print if all tests have passed or not
    if (passedAllTests){
        printf("ALL TESTS PASSED\n");
    }
    else{
        printf("ONE OR MORE TESTS FAILED\n");
    }
    return 0;
}

// If expression is false, allows you to continue program and print error message
int myAssert(int expression, int* passedAllTests, char message[]){
    if(expression){
        return 1;
    }
    printf("\nTEST FAILED: %s\nEnter to continue / 'E' to exit.\n\n", message);
    *passedAllTests = 0;
    /*
    char input;
    input = getchar();
    if (input == 'e' || input == 'E'){
        exit(1);
    }
    */
    return 0;
}
// Cards number is passed and the card name is returned
char* getCardName(int card){
    if (card == 0){return "curse";}
    if (card == 1){return "estate";}
    if (card == 2){return "duchy";}
    if (card == 3){return "province";}
    if (card == 4){return "copper";}
    if (card == 5){return "silver";}
    if (card == 6){return "gold";}
    if (card == 7){return "adventurer";}
    if (card == 8){return "council_room";}
    if (card == 9){return "feast";}
    if (card == 10){return "gardens";}
    if (card == 11){return "mine";}
    if (card == 12){return "remodel";}
    if (card == 13){return "smithy";}
    if (card == 14){return "village";}
    if (card == 15){return "baron";}
    if (card == 16){return "great_hall";}
    if (card == 17){return "minion";}
    if (card == 18){return "steward";}
    if (card == 19){return "tribute";}
    if (card == 20){return "ambassador";}
    if (card == 21){return "cutpurse";}
    if (card == 22){return "embargo";}
    if (card == 23){return "outpost";}
    if (card == 24){return "salvager";}
    if (card == 25){return "sea_hag";}
    if (card == 26){return "treasure_map";}
    return NULL;
}

// Compare members of two game states
void testStates(struct gameState* state, struct gameState* testState, int* passedAllTests){
    int i, j;
    myAssert(state->numPlayers == testState->numPlayers, passedAllTests, "numPlayers");
    for (i = 0; i < treasure_map+1; i++){
        myAssert(state->supplyCount[i] == testState->supplyCount[i], passedAllTests, "supplyCount");
        myAssert(state->embargoTokens[i] == testState->embargoTokens[i], passedAllTests, "embargoTokens");
    }
    myAssert(state->outpostPlayed == testState->outpostPlayed, passedAllTests, "outpostPlayed");
    myAssert(state->outpostTurn == testState->outpostTurn, passedAllTests, "outpostTurn");
    myAssert(state->whoseTurn == testState->whoseTurn, passedAllTests, "whoseTurn");
    myAssert(state->phase == testState->phase, passedAllTests, "phase");
    myAssert(state->numActions == testState->numActions, passedAllTests, "actions");
    myAssert(state->coins == testState->coins, passedAllTests, "coins");
    myAssert(state->numBuys == testState->numBuys, passedAllTests, "numBuys");
    for (i = 0; i < MAX_PLAYERS; i++){
        myAssert(state->handCount[i] == testState->handCount[i], passedAllTests, "handCount");
        myAssert(state->deckCount[i] == testState->deckCount[i], passedAllTests, "deckCount");
        myAssert(state->discardCount[i] == testState->discardCount[i], passedAllTests, "discardCount");

        for(j = 0; j < state->handCount[i]; j++){
            myAssert(state->hand[i][j] == testState->hand[i][j], passedAllTests, "hand");
            myAssert(state->deck[i][j] == testState->deck[i][j], passedAllTests, "deck");
            myAssert(state->discard[i][j] == testState->discard[i][j], passedAllTests, "discard");
        }
    }
    myAssert(state->playedCardCount == testState->playedCardCount, passedAllTests, "playedCardCount");
    for (i = 0; i < state->playedCardCount; i++){
        myAssert(state->playedCards[i] == testState->playedCards[i], passedAllTests, "playedCards");
    }
}
