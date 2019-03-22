/*********************************************************************
** Program name:    testFullDeckCount
** Author:          Joseph Sullivan, sullijos
** Date:            10/22/2018
** Description:     Unit test for function
**                  int fullDeckCount(int player, int card, struct gameState *state) {
**                      int i;
**                      int count = 0;
**                      for (i = 0; i < state->deckCount[player]; i++)
**                      {
**                          if (state->deck[player][i] == card) count++;
**                      }
**                      for (i = 0; i < state->handCount[player]; i++)
**                      {
**                          if (state->hand[player][i] == card) count++;
**                      }
**                      for (i = 0; i < state->discardCount[player]; i++)
**                      {
**                          if (state->discard[player][i] == card) count++;
**                      }
**                      return count;
**                  }
**                  Tests by setting all cards in all piles to curse cards,
**                  then the size of the hand, deck, and discard piles are increased,
**                  and counts are checked with fullDeckCount(). A second test replaces
**                  the curses with treasure maps and performs a fullDeckCount()
**                  for curses and treasure maps after replacing each curse.
*********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

// Prototypes
int myAssert(int expression, int* passedAllTests, char message[]);
void testStates(struct gameState* state, struct gameState* testState, int* passedAllTests);
void testAndPrint(int* passedAllTests, char* cardName, int card, int p, struct gameState *state, int hand, int deck, int discard);

int main(){
    int passedAllTests = 1;
    int kingdomCards[10] = {adventurer, council_room, feast, gardens, mine
                            , remodel, smithy, village, baron, great_hall};
    int seed = 1;

    // Initialize game
    struct gameState state;
    memset(&state, 0, sizeof(struct gameState));
    initializeGame(MAX_PLAYERS, kingdomCards, seed, &state);

    // Set each players hand, deck, and discard piles to curse cards
    int i;
    for (i = 0; i < MAX_PLAYERS; i += (MAX_PLAYERS - 1)){
        int j;
        for (j = 0; j < MAX_HAND; j++){
            state.hand[i][j] = curse;
            state.deck[i][j] = curse;
            state.discard[i][j] = curse;
        }
    }

    // Set all hand, deck, and discard sizes to 0
    for (i = 0; i < MAX_PLAYERS; i += (MAX_PLAYERS - 1)){
        state.handCount[i] = 0;
        state.deckCount[i] = 0;
        state.discardCount[i] = 0;
    }

    // Begin test
    printf("TESTING fullDeckCount():\n");

    // declare pile counts for each player
    int handCurse[MAX_PLAYERS];
    int deckCurse[MAX_PLAYERS];
    int discardCurse[MAX_PLAYERS];
    for (i = 0; i < MAX_PLAYERS; i += (MAX_PLAYERS - 1)){
        printf("\nTesting for player %d\n", i + 1);
        // All counts start at 0, test counts
        handCurse[i] = 0;
        deckCurse[i] = 0;
        discardCurse[i] = 0;
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        // Add one to each pile and test
        state.handCount[i] = 1;
        state.hand[i][0] = curse;
        handCurse[i] = 1;
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        state.deckCount[i] = 1;
        state.deck[i][0] = curse;
        deckCurse[i] = 1;
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        state.discardCount[i] = 1;
        state.discard[i][0] = curse;
        discardCurse[i] = 1;
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);

        // Fill remainder of hand, deck, discard pile with curse cards and test
        int j;
        for (j = 1; j < MAX_DECK; j++){
            state.hand[i][j] = curse;
            state.handCount[i]++;
            handCurse[i]++;
        }
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        for (j = 1; j < MAX_DECK; j++){
            state.deck[i][j] = curse;
            state.deckCount[i]++;
            deckCurse[i]++;
        }
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        for (j = 1; j < MAX_DECK; j++){
            state.discard[i][j] = curse;
            state.discardCount[i]++;
            discardCurse[i]++;
        }
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
    }

    printf("\nReplacing curses with treasure_maps, first 1, then all");
    int handMap[MAX_PLAYERS];
    int deckMap[MAX_PLAYERS];
    int discardMap[MAX_PLAYERS];
    for (i = 0; i < MAX_PLAYERS; i += (MAX_PLAYERS - 1)){
        printf("\nTesting for player %d\n", i + 1);
        state.hand[i][0] = treasure_map;
        handMap[i] = 1;
        handCurse[i]--;
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        testAndPrint(&passedAllTests, "treasure_map", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);

        state.deck[i][0] = treasure_map;
        deckMap[i] = 1;
        deckCurse[i]--;
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        testAndPrint(&passedAllTests, "treasure_map", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);

        state.discard[i][0] = treasure_map;
        discardMap[i] = 1;
        discardCurse[i]--;
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        testAndPrint(&passedAllTests, "treasure_map", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        int j;
        for (j = 1; j < MAX_DECK; j++){
            state.hand[i][j] = treasure_map;
            handMap[i]++;
            handCurse[i]--;
        }
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        testAndPrint(&passedAllTests, "treasure_map", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        for (j = 1; j < MAX_DECK; j++){
            state.deck[i][j] = treasure_map;
            deckMap[i]++;
            deckCurse[i]--;
        }
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        testAndPrint(&passedAllTests, "treasure_map", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        for (j = 1; j < MAX_DECK; j++){
            state.discard[i][j] = treasure_map;
            discardMap[i]++;
            discardCurse[i]--;
        }
        testAndPrint(&passedAllTests, "curse", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
        testAndPrint(&passedAllTests, "treasure_map", curse, i, &state, handCurse[i], deckCurse[i], discardCurse[i]);
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
// Tests deck count and prints results
void testAndPrint(int* passedAllTests, char* cardName, int card, int p, struct gameState *state, int hand, int deck, int discard){
    struct gameState testState;
    printf("%s in hand: %d\nin deck: %d\nin discard: %d\nTotal: %d\n",
    cardName, hand, deck, discard, hand+deck+discard);
    memset(&testState, 0, sizeof(struct gameState));
    memcpy(&testState, state, sizeof(struct gameState));
    printf("Result of fullDeckCount(): %d\n", fullDeckCount(p, card, state));
    myAssert(fullDeckCount(p, card, state) == hand+deck+discard, passedAllTests
        , "Sum of cards in each pile does not match what was set\n");
    testStates(state, &testState, passedAllTests);

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
