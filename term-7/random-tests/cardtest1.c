/*********************************************************************
** Program name:    testAdventurer
** Author:          Joseph Sullivan, sullijos
** Date:            10/22/2018
** Description:     Unit test for adventurer card
*********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

#define TEST_CARD adventurer

char* getCardName(int card);
int myAssert(int expression, int* passedAllTests, char message[]);

int main(){
    printf("TESTING ADVENTURER CARD\n\n");
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
    int coinsGained = 2;
    int i;

    for (i = 0; i < numPlayers; i++){
        int player = i;
        int j;
        state.whoseTurn = player;
        // Make test card the only card in player's hand
        printf("Player %d's hand set to a single test card\n", player + 1);
        state.handCount[player] = 1;
        state.hand[player][0] = TEST_CARD;

        // Save game state for comparison after card is played
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
        printf("Cards in played pile before playing test card: %d, expected: %d\n", state.playedCardCount, numPlayedCards);
        myAssert(state.playedCardCount == numPlayedCards, &passedAllTests, "Wrong played count");
        numPlayedCards++;
        // Cards in played pile after playing test card
        printf("Cards in played pile after playing test card: %d, expected: %d\n", testState.playedCardCount, numPlayedCards);
        myAssert(testState.playedCardCount == numPlayedCards, &passedAllTests, "Wrong played count");
        // Is test card the card in played pile?
        printf("Cards in played pile:\n");
        for (j = 0; j < testState.playedCardCount; j++){
            printf("%s\n", getCardName(testState.playedCards[j]));
        }
        printf("Cards expected in played pile: %s\n", getCardName(TEST_CARD));
        myAssert(testState.playedCards[numPlayedCards - 1] == TEST_CARD, &passedAllTests, "Wrong cards");
        // number of cards in deck before playing test card
        printf("Cards in deck before playing test card: %d\n", state.deckCount[player]);
        // number of cards in deck after playing test card
        int deckExpected = state.deckCount[player] - testState.discardCount[player] - coinsGained;
        printf("Cards in deck after playing test card: %d, expected: %d\n", testState.deckCount[player], deckExpected);
        myAssert(testState.deckCount[player] == deckExpected, &passedAllTests, "Wrong deck count");

        // number of cards in hand before playing test card
        printf("Cards in hand before playing test card: %d\n", state.handCount[player]);
        // number of cards in hand after playing test card
        printf("Cards in hand after playing test card: %d, expected: %d\n", testState.handCount[player], coinsGained);
        myAssert(testState.handCount[player] == coinsGained, &passedAllTests, "Wrong hand count");
        // number of cards in discard before playing test card
        printf("Cards in discard before playing test card: %d\n", state.discardCount[player]);
        // number of cards in discard after playing test card
        int expectedDiscard = state.deckCount[player] - testState.deckCount[player] - coinsGained;
        printf("Cards in discard after playing test card: %d, expected: %d\n"
        , testState.discardCount[player], expectedDiscard);
        myAssert(testState.discardCount[player] == expectedDiscard, &passedAllTests, "wrong discard count");
        printf("Making sure hand has only coins:\n");
        for (j = 0; j < testState.handCount[player]; j++){
            int card = testState.hand[player][j];
            printf("%s\n", getCardName(card));
            myAssert(card == copper || card == silver || card == gold, &passedAllTests, "cards other than coins drawn");
        }
        printf("Making sure only non-coins were discarded:\n");
        for (j = 0; j < testState.discardCount[player]; j++){
            int card = testState.discard[player][j];
            printf("%s\n", getCardName(card));
            myAssert(card != copper && card != silver && card != gold, &passedAllTests, "coins discarded");
        }

        printf("Making sure all other game states are unchanged\n");
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
        for(j = 0; j < state.discardCount[player]; j++){
            myAssert(state.discard[player][j] == testState.discard[player][j], &passedAllTests, "discard");
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
