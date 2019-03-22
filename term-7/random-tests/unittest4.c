/*********************************************************************
** Program name:    testNumHandCards
** Author:          Joseph Sullivan, sullijos
** Date:            10/21/2018
** Description:     Unit test for function
**                  int numHandCards(struct gameState *state){
**                      return state->handCount[ whoseTurn(state) ];
**                  }
**                  Tests use of the function with all combinations of
**                  of min and max player number and min and max hand size.
*********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

// prototypes
int myAssert(int expression, int* passedAllTests, char message[]);
void testStates(struct gameState* state, struct gameState* testState, int* passedAllTests);

int main(){
    // Declare variables
    int passedAllTests = 1;
    int kingdomCards[10] = {adventurer, council_room, feast, gardens, mine
                            , remodel, smithy, village, baron, great_hall};
    int seed = 1;
    struct gameState state, testState;

    // Initialize game and make copy of state for testing
    memset(&state, 0, sizeof(struct gameState));
    initializeGame(MAX_PLAYERS, kingdomCards, seed, &state);

    printf("Testing numHandCards():\n"); // Begin test

    // Test min and max for player number and hand size
    int i;
    for (i = 0; i < MAX_PLAYERS; i += (MAX_PLAYERS - 1)){
        printf("Testing for player %d\n", i + 1);
        state.whoseTurn = i; // set whose turn it is

        int j;
        for (j = 0; j <= MAX_HAND; j += MAX_HAND){
            state.handCount[i] = j; // Set number of cards in hand

            // Copy game state for comparison after test function is called
            memset(&testState, 0, sizeof(struct gameState));
            memcpy(&testState, &state, sizeof(struct gameState));

            // Test that correct hand count is returned
            printf("Player %d: cards in hand = %d, Expected = %d\n", whoseTurn(&testState) + 1, numHandCards(&testState), j);
            myAssert(numHandCards(&testState) == j, &passedAllTests, "Wrong number of cards in hand");

            // Test game states
            testStates(&state, &testState, &passedAllTests);
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
