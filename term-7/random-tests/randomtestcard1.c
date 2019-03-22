/*********************************************************************
** Program name:    randomtestcard1
** Author:          Joseph Sullivan, sullijos
** Date:            11/13/2018
** Description:     random test for smithy card
*********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"
#include <time.h>

# define NOISY_TEST 0
# define NUM_TESTS 20000

char* getCardName(int card);
int myAssert(int expression, int* passedAllTests, char message[]);

// put smithy in hand, copy game state, test results
void testSmithy(int player, struct gameState *post, int *passedAllTests) {
    int i, j, handPos, result, availableCards, preCount, postCount, trashFlag;
    int cardsDrawn = 3;
    int choice1, choice2, choice3, bonus;
    choice1 = choice2 = choice3 = bonus = trashFlag = 0;
    struct gameState pre;

    // Place smithy in random hand position.
    handPos = floor(Random() * post->handCount[player]);
    post->hand[player][handPos] = smithy;

    // Copy game state before playing card
    memcpy(&pre, post, sizeof(struct gameState));

    // Play smithy card
    result = cardEffect(smithy, choice1, choice2, choice3, post, handPos, &bonus);

    // Check how many cards are available to draw
    availableCards = pre.deckCount[player] + pre.discardCount[player];

    // Set expected number of cards drawn
    if (availableCards < 3){
        cardsDrawn = availableCards;
    }

    // Print state of hand, discard, deck and number of cards that should be drawn
    if(NOISY_TEST){printf("Cards in hand before playing Smithy: %d\n", pre.handCount[player]);}
    if(NOISY_TEST){printf("Available cards in discard + deck: %d\n", availableCards);}
    if(NOISY_TEST){printf("Number of cards that should be drawn: %d, net gain of %d\n", cardsDrawn, cardsDrawn - 1);}

    // check number of cards in hand after smithy is played
    if(NOISY_TEST){printf("Cards in hand: %d, Expected: %d\n", post->handCount[player], pre.handCount[player] + cardsDrawn - 1);}
    myAssert(post->handCount[player] == (pre.handCount[player] + cardsDrawn - 1), passedAllTests, "Wrong hand count");

    //  check that smithy is in played card pile
    if(NOISY_TEST){printf("Last card in played cards: %s, Expected: smithy\n", getCardName(post->playedCards[pre.playedCardCount]));}
    myAssert(post->playedCards[pre.playedCardCount] == smithy, passedAllTests, "Smithy not in played card pile");

    // Check that played card pile has increased by 1
    if(NOISY_TEST){printf("Played cards before: %d, should gain 1, after: %d\n", pre.playedCardCount, post->playedCardCount);}
    myAssert(post->playedCardCount == (pre.playedCardCount + 1), passedAllTests, "Played card count didn't increase by 1");

    // count of cards in discard and deck == pre discard + deck - cardsDrawn
    preCount = pre.discardCount[player] + pre.deckCount[player];
    postCount = post->discardCount[player] + post->deckCount[player];
    if(NOISY_TEST){printf("Cards in deck + discard: %d, Expected: %d\n", postCount, preCount - cardsDrawn);}
    myAssert(postCount == (preCount - cardsDrawn), passedAllTests, "Wrong number of cards in deck + discard");

    // Test that result is as expected
    myAssert(result == 0, passedAllTests, "cardEffect returned non-zero");

    // Test that other states are the same
    myAssert(pre.numPlayers == post->numPlayers, passedAllTests, "numPlayers changed");
    for (i = 0; i < treasure_map + 1; i++){
        myAssert(pre.supplyCount[i] == post->supplyCount[i], passedAllTests, "Supply count changed");
        myAssert(pre.embargoTokens[i] == post->embargoTokens[i], passedAllTests, "Embargo token changed");
    }
    myAssert(pre.outpostPlayed == post->outpostPlayed, passedAllTests, "outpostPlayed changed");
    myAssert(pre.outpostTurn == post->outpostTurn, passedAllTests, "outpostTurn changed");
    myAssert(pre.whoseTurn == post->whoseTurn, passedAllTests, "Whose turn changed");
    myAssert(pre.phase == post->phase, passedAllTests, "phase changed");
    myAssert(pre.numActions == post->numActions, passedAllTests, "numActions changed");
    myAssert(pre.whoseTurn == post->whoseTurn, passedAllTests, "Whose turn changed");
    myAssert(pre.coins == post->coins, passedAllTests, "coins changed");
    myAssert(pre.numBuys == post->numBuys, passedAllTests, "numBuys changed");

    for (i = 0; i < MAX_PLAYERS; i++){
        if (pre.whoseTurn != i){
            myAssert(pre.handCount[i] == post->handCount[i], passedAllTests, "Other player's hand count changed");
            for(j = 0; j < MAX_HAND; j++){
                myAssert(pre.hand[i][j] == post->hand[i][j], passedAllTests, "One of other player's hand cards changed");
            }
            myAssert(pre.deckCount[i] == post->deckCount[i], passedAllTests, "Other player's deck count changed");
            for(j = 0; j < MAX_DECK; j++){
                myAssert(pre.deck[i][j] == post->deck[i][j], passedAllTests, "One of other player's deck cards changed");
            }
            myAssert(pre.discardCount[i] == post->discardCount[i], passedAllTests, "Other player's discard count changed");
            for(j = 0; j < MAX_DECK; j++){
                myAssert(pre.discard[i][j] == post->discard[i][j], passedAllTests, "One of other player's discard cards changed");
            }
        }
    }
}

int main () {
  int n, i, j, player, totalCards, passedAllTests;
  struct gameState state;
  passedAllTests = 1;

  // Seed random number generation
  PlantSeeds((long int)time(0));

  printf ("Testing Smithy.\n");
  printf ("RANDOM TESTS.\n");

  for (n = 0; n < NUM_TESTS; n++) {

    // Randomize game state
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&state)[i] = floor(Random() * 256);
    }

    // choose player
    player = floor(Random() * MAX_PLAYERS);
    state.whoseTurn = player;

    // set pile sizes so that total cards doesn't exceed MAX_DECK
    totalCards = MAX_DECK + 1;
    state.handCount[player] = floor(Random() * MAX_DECK) + 1;
    totalCards -= state.handCount[player];
    state.deckCount[player] = floor(Random() * totalCards);
    totalCards -= state.deckCount[player];
    state.discardCount[player] = floor(Random() * totalCards);
    totalCards -= state.discardCount[player];
    state.playedCardCount = floor(Random() * totalCards);

    // Make sure there is room for smithy to be added to played Cards
    if (state.playedCardCount >= MAX_DECK){
        state.playedCardCount--;
    }

    // Add random cards to each pile
    for (j = 0; j < state.deckCount[player]; j++){
        state.deck[player][j] = floor(Random() * treasure_map + 1);
    }
    for (j = 0; j < state.discardCount[player]; j++){
        state.discard[player][j] = floor(Random() * treasure_map + 1);
    }
    for (j = 0; j < state.handCount[player]; j++){
        state.hand[player][j] = floor(Random() * treasure_map + 1);
    }
    for (j = 0; j < state.playedCardCount; j++){
        state.playedCards[j] = floor(Random() * treasure_map + 1);
    }
    if(NOISY_TEST){printf("TEST: %d\n", n + 1);}
    testSmithy(player, &state, &passedAllTests);
  }

  if(passedAllTests){
      printf ("ALL TESTS OK\n");
  }
  else{
      printf("SOME TESTS FAILED\n");
  }

  return 0;
}

// returns card name for corresponding card number
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
    return "(invalid)";
}

// If expression is false, allows you to continue program and print error message
int myAssert(int expression, int* passedAllTests, char message[]){
    if(expression){
        return 1;
    }
    printf("TEST FAILED: %s\n", message);
    //printf("Enter to continue / 'E' to exit.\n\n");
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
