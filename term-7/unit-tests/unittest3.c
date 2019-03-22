/*********************************************************************
** Program name:    testKingdomCards
** Author:          Joseph Sullivan, sullijos
** Date:            10/22/2018
** Description:     Unit test for function
**                  int* kingdomCards(int k1, int k2, int k3, int k4, int k5, int k6, int k7,
**                                    int k8, int k9, int k10) {
**                      int* k = malloc(10 * sizeof(int));
**                      k[0] = k1;
**                      k[1] = k2;
**                      k[2] = k3;
**                      k[3] = k4;
**                      k[4] = k5;
**                      k[5] = k6;
**                      k[6] = k7;
**                      k[7] = k8;
**                      k[8] = k9;
**                      k[9] = k10;
**                      return k;
**                  }
**                  Tests setting kingdom cards with function.
**                  Runs tests with all possible kingdom cards.
*********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define NUM_K_CARDS 10

char* getCardName(int card);
int myAssert(int expression, int* passedAllTests, char message[]);

int main(){
    int passedAllTests = 1;
    int* kCards;

    // Begin test
    printf("TESTING kingdomCards():\n");

    // Assign first 10 kingdom cards with kingdomCards function
    kCards = kingdomCards(adventurer, council_room, feast, gardens, mine
                          , remodel, smithy, village, baron, great_hall);

// Compare results to what is expected
    printf("   Expected kingdom cards: %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n"
           , getCardName(adventurer), getCardName(council_room), getCardName(feast)
           , getCardName(gardens), getCardName(mine), getCardName(remodel)
           , getCardName(smithy), getCardName(village), getCardName(baron)
           , getCardName(great_hall));
    printf("Contents of kingdom cards: %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n"
           , getCardName(kCards[0]), getCardName(kCards[1]), getCardName(kCards[2])
           , getCardName(kCards[3]), getCardName(kCards[4]), getCardName(kCards[5])
           , getCardName(kCards[6]), getCardName(kCards[7]), getCardName(kCards[8])
           , getCardName(kCards[9]));
    int i;
    for(i = 0; i < NUM_K_CARDS; i++){
        myAssert(kCards[i] == adventurer + i, &passedAllTests, "Kingdom card does not match what was set\n");
    }

    free(kCards); // free memory allocated in kingdomCards()

    // Assign last 10 kingdom cards with kingdomCards function
    kCards = kingdomCards(minion, steward, tribute, ambassador, cutpurse, embargo
                          , outpost, salvager, sea_hag, treasure_map);

// Compare results to what is expected
    printf("   Expected kingdom cards: %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n"
           , getCardName(minion), getCardName(steward), getCardName(tribute)
           , getCardName(ambassador), getCardName(cutpurse), getCardName(embargo)
           , getCardName(outpost), getCardName(salvager), getCardName(sea_hag)
           , getCardName(treasure_map));
    printf("Contents of kingdom cards: %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n"
           , getCardName(kCards[0]), getCardName(kCards[1]), getCardName(kCards[2])
           , getCardName(kCards[3]), getCardName(kCards[4]), getCardName(kCards[5])
           , getCardName(kCards[6]), getCardName(kCards[7]), getCardName(kCards[8])
           , getCardName(kCards[9]));
    for(i = 0; i < NUM_K_CARDS; i++){
        myAssert(kCards[i] == minion + i, &passedAllTests, "Kingdom card does not match what was set\n");
    }

    free(kCards); // free memory allocated in kingdomCards()

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
