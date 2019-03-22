/*********************************************************************
** Program name:    testSmithy
** Author:          Joseph Sullivan, sullijos
** Date:            10/22/2018
** Description:     Unit test for Smithy card
*********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

#define TEST_CARD smithy

char* getCardName(int card);
int myAssert(int expression, int* passedAllTests, char message[]);

int main(){
    printf("TESTING SMITHY CARD\n\n");

    // Variables for initializing game
    int numPlayers = 2;
    int seed = 33343455;
    int kingdomCards[10] = {adventurer, embargo, village, minion, mine, cutpurse,
    			            sea_hag, tribute, smithy, council_room};
    struct gameState state, testState;

    // Initialize game
    memset(&state, 0, sizeof(struct gameState));
    initializeGame(numPlayers, kingdomCards, seed, &state);

    // Variables for testing
    int passedAllTests = 1; // Flag for printing final result of test
    int i;

    for (i = 0; i < numPlayers; i++){
        int player = i;
        int j;
        state.whoseTurn = player;
        // Make test card the only card in player's hand
        printf("Player %d's hand set to a single copy of test card\n", player + 1);
        state.handCount[player] = 1;
        state.hand[player][0] = TEST_CARD;

        // Save game state for comparison after test card is played
        memset(&testState, 0, sizeof(struct gameState));
        memcpy(&testState, &state, sizeof(struct gameState));

        // variables for calling carEffect()
        int handPos = 0;
        int choice1 = 0;
        int choice2 = 0;
        int choice3 = 0;
        int bonus = 0;

        // Play test card
        cardEffect(TEST_CARD, choice1, choice2, choice3, &testState, handPos, &bonus);

        /****** Confirm that test card ends up in played pile after playing it ******/
        int numPlayedCards = 0;
        // Cards in played pile before playing test card
        printf("Cards in played pile before playing test card %d\n", state.playedCardCount);
        myAssert(state.playedCardCount == numPlayedCards, &passedAllTests, "Wrong number of cards in played pile");
        numPlayedCards++;
        // Cards in played pile after playing test card
        printf("Cards in played pile after playing test card: %d\n", testState.playedCardCount);
        myAssert(testState.playedCardCount == numPlayedCards, &passedAllTests, "Wrong number of cards in played pile");
        // Is test card the card in played pile?
        printf("Cards in played pile:\n");
        for (j = 0; j < testState.playedCardCount; j++){
            printf("%s\n", getCardName(testState.playedCards[j]));
        }
        myAssert(testState.playedCards[numPlayedCards - 1] == TEST_CARD, &passedAllTests, "Cards in played pile don't match");

        /****** Confirm that drawn cards come from player 1's deck and end up in player 1's hand ******/
        int smithyDraw = 3;
        int topDeckIndex = state.deckCount[player] - 1;
        int cardsInHand = 1;

        //Cards in deck before playing test card
        printf("Player %d cards in deck before playing test card: %d\n", player + 1, state.deckCount[player]);
        // Cards in deck after playing test card
        printf("Player %d cards in deck after playing test ard: %d\n", player + 1, testState.deckCount[player]);
        myAssert(testState.deckCount[player] == (state.deckCount[player] - smithyDraw), &passedAllTests, "Wrong deck count");
        // Cards in hand before playing test card
        printf("Player %d cards in hand before playing test card: %d\n", player + 1, state.handCount[player]);
        myAssert(state.handCount[player] == cardsInHand, &passedAllTests, "Wrong hand count");
        // Cards in hand after playing test card
        cardsInHand = cardsInHand - 1 + smithyDraw;
        printf("Player %d cards in hand after playing test card: %d\n", player + 1, testState.handCount[player]);
        myAssert(testState.handCount[player] == cardsInHand, &passedAllTests, "Wrong hand count");

        //Top three cards of player 1's deck
        printf("Top 3 cards of deck before playing test card:\n");
        for (j = topDeckIndex; j > topDeckIndex - smithyDraw; j--){
            printf("%s\n", getCardName(state.deck[player][j]));
        }
        // Cards in hand after playing test card
        printf("Cards in hand after playing test card:\n");
        for (j = 0; j < testState.handCount[player]; j++){
            printf("%s\n", getCardName(testState.hand[player][j]));
            if (j + 1 < testState.handCount[player]){
                myAssert(testState.hand[player][j + 1] == state.deck[player][state.deckCount[player] - j - 1], &passedAllTests, "Cards don't match");
            }
            else{
                myAssert(testState.hand[player][0] == state.deck[player][state.deckCount[player] - j - 1], &passedAllTests, "Cards don't match");
            }
        }

        printf("Checking that other game states have remained the same.\n");
        if (player == 0){
            player = 1;
        }
        else{
            player = 0;
        }
        myAssert(state.numPlayers == testState.numPlayers, &passedAllTests, "numPlayers");
        for (j = 0; j < treasure_map+1; j++){
            myAssert(state.supplyCount[j] == testState.supplyCount[j], &passedAllTests, "supplyCount");
            myAssert(state.embargoTokens[j] == testState.embargoTokens[j], &passedAllTests, "embargoTokens");
        }
        myAssert(state.outpostPlayed == testState.outpostPlayed, &passedAllTests, "outpostPlayed");
        myAssert(state.outpostTurn == testState.outpostTurn, &passedAllTests, "outpostTurn");
        myAssert(state.whoseTurn == testState.whoseTurn, &passedAllTests, "whoseTurn");
        myAssert(state.phase == testState.phase, &passedAllTests, "phase");
        myAssert(state.numActions == testState.numActions, &passedAllTests, "actions");
        myAssert(state.coins == testState.coins, &passedAllTests, "coins");
        myAssert(state.numBuys == testState.numBuys, &passedAllTests, "numBuys");
        myAssert(state.handCount[player] == testState.handCount[player], &passedAllTests, "handCount");
        for(j = 0; j < state.handCount[player]; j++){
            myAssert(state.hand[player][j] == testState.hand[player][j], &passedAllTests, "hand");
        }
        myAssert(state.deckCount[player] == testState.deckCount[player], &passedAllTests, "deckCount");
        for(j = 0; j < state.deckCount[player]; j++){
            myAssert(state.deck[player][j] == testState.deck[player][j], &passedAllTests, "deck");
        }
        for (j = 0; j < numPlayers; j++){
            player = j;
            myAssert(state.discardCount[player] == testState.discardCount[player], &passedAllTests, "discardCount");
            int k;
            for(k = 0; k < state.discardCount[player]; k++){
                myAssert(state.discard[player][k] == testState.discard[player][k], &passedAllTests, "discard");
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
