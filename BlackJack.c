#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <time.h>

//////////////////////////////// DEFINES ////////////////////////////////

#define NUMBER_OF_SUITS             4
#define NUMBER_OF_CARDS_PER_SUIT    13

typedef struct Card
{
    char suit[5]; 
    char number[3];
    int value;
} Card;

typedef enum States
{
    startGame = 0,
    gameOver,
    quitGame,
    dealFirstHand,
    handlePlayer,
    handleHouse,
    handleCredit
}States;

typedef struct Player
{
    char name[10];
    Card cardsInHand[5];
    unsigned short int numOfCards;
    unsigned short int sumOfCards;
    int credit;
    int bidValue;
    struct playerState {
        unsigned char isStaying : 1;
        unsigned char isBust : 1;
        unsigned char isWinner : 1;
        unsigned char hasAInHand : 1;
        unsigned char BLACKJACK: 1;
        unsigned char _reserved : 3;
    } playerstate;
}Player;

typedef Card DrawDeck[52];
//////////////////////////////// DEFINES ////////////////////////////////

//////////////////////////////// SEEDS ////////////////////////////////

int seedCards[13] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 10, 10, 10};
char nameSeedCards[13][3] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "A", "J", "Q", "K"};
char suitSeeds[5][5] = {"♥", "♠", "♣", "♦"};

//////////////////////////////// SEEDS ////////////////////////////////

//////////////////////////////// FUNCTION PROTOTYPES ////////////////////////////////

void generateDeck(void);
void printDeck(void);
void deleteFromDeck(Card card);
void putCardInPlayersHand(int round, Card card);
void handleThisPlayer(int playerNo);
void showPlayersHand(int playerNo);
void handlePlayerCredit(int playerNo);
Player generatePlayer(int position);
Card dealCard(void);

//////////////////////////////// FUNCTION PROTOTYPES ////////////////////////////////

//////////////////////////////// GLOBALS ////////////////////////////////

DrawDeck drawDeck;
unsigned short int cardsInDrawDeck = 52;
Player Players[10];
unsigned short int numberOfPlayers = 0;
States state;

//////////////////////////////// GLOBALS ////////////////////////////////

//////////////////////////////// IMPLEMENTATION ////////////////////////////////

void initFunction(void)
{
    srand(time(NULL));
    generateDeck();
    state = startGame;
}

void testFunction(void)
{
    Players[5] = generatePlayer(5);
    Players[6] = generatePlayer(6);
    Players[7] = generatePlayer(7);

    Players[5].credit = 10000;
    Players[5].bidValue = 50;
    Players[6].credit = 10000;
    Players[6].bidValue = 50;
    Players[7].credit = 10000;
    Players[7].bidValue = 50;

    Card card1 = {"♥", "A", 11};
    Card card2 = {"♥", "Q", 10};
    Card card3 = {"♥", "2", 2};
    Card card4 = {"♥", "9", 9};
    Card card5 = {"♥", "8", 8};
    
    putCardInPlayersHand(5, card1);   
    putCardInPlayersHand(5, card2);
    handlePlayerCredit(5);
    
    putCardInPlayersHand(6, card3);
    putCardInPlayersHand(6, card4);
    putCardInPlayersHand(6, card2);
    handlePlayerCredit(6);

    putCardInPlayersHand(7, card4);
    putCardInPlayersHand(7, card5);
    putCardInPlayersHand(7, card2);
    handlePlayerCredit(7);
}

int main(void)
{    
    char a = ' ';
    initFunction();
    // testFunction();
    while(state != gameOver)
    {
        switch (state)
        {
        case startGame:
            /* code */
            printf("introduceti s pentru a incepe jocul sau q pentru a-l parasi: ");
            scanf(" %c", &a);
            getchar();
            printf("\n");
            switch (a)
            {
            case 's':
                /* code */
                printf("introduceti numarul de jucatori: ");
                scanf(" %d", &numberOfPlayers);
                printf("\n");

                for(int i = 0; i < numberOfPlayers; i++)
                {
                    Players[i] = generatePlayer(i);
                }

                printf("Fiecare jucator incepe cu 10 000 de credite!\n");

                for(int i = 0; i < numberOfPlayers; i++)
                {
                    Players[i].credit = 10000;
                }

                for(int i = 1; i < numberOfPlayers; i++)
                {
                    printf("%s: Cat vreti sa pariati? ", Players[i].name);
                    scanf(" %d", &Players[i].bidValue);
                    printf("\n");
                }

                
                
                state = dealFirstHand;
                break;
            
            case 'q':
                /* code */                
                state = quitGame;
                break;

            default:
            
                printf("WRONG!\n");
                //state = startGame;
                break;
            }            
            break;

        case quitGame:
            /* code */            
            printf("Ciuss bye!\n");
            state = gameOver;
            break;
        
        case dealFirstHand:
            /* code */
            for(int i = 0; i < 2; i++) //two cards per player
            {
                for(int j = 0; j < numberOfPlayers; j++)
                {
                    putCardInPlayersHand(j, dealCard());
                }
            }
            state = handlePlayer;
            break;

        case handlePlayer:
            /* code */
            for(int i = 1; i < numberOfPlayers; i++) 
            {
                handleThisPlayer(i);                
            }
            state = handleHouse;
            break;

        case handleHouse:
            /* code */
            handleThisPlayer(0);
            state = handleCredit;
            break;

        case handleCredit:
            /* code */
            for(int i = 1; i < numberOfPlayers; i++)
            {
                handlePlayerCredit(0);
            }

            for(int i = 1; i < numberOfPlayers; i++)
            {
                printf("%s mai are %d banile\n", Players[i].name, Players[i].credit);
            }
            state = gameOver;
            break;
        
        default:
            break;
        }
    }

    return 0;
}

void generateDeck(void)
{
    for(int i = 0; i < NUMBER_OF_SUITS; i++)
    {
        for(int j = 0; j < NUMBER_OF_CARDS_PER_SUIT; j++)
        {
            strcpy(drawDeck[i*13+j].number ,nameSeedCards[j]);
            strcpy(drawDeck[i*13+j].suit ,suitSeeds[i]);
            drawDeck[i*13+j].value = seedCards[j];
        }
    }
}

Player generatePlayer(int position)
{
    Player player;
    sprintf (player.name, "Player_%d", position);
    player.numOfCards = 0;
    player.sumOfCards = 0;
    player.playerstate.isBust = 0;
    player.playerstate.isStaying = 0;
    player.playerstate.isWinner = 0;
    player.playerstate.hasAInHand = 0;
    player.credit = 0;
    player.bidValue = 0;
    // printf("generated player: %s\n", player.name);
    return player;
}

void printDeck(void)
{
    for(int i = 0; i < NUMBER_OF_SUITS; i++)
    {
        for(int j = 0; j < NUMBER_OF_CARDS_PER_SUIT; j++)
        {
            printf("%s%s ", drawDeck[i*13+j].number, (drawDeck[i*13+j].suit));
        }
        printf("\n");        
    } 
}

void deleteFromDeck(Card card)
{
    for(int i = 0; i < cardsInDrawDeck; i++)
    {
        if(!strcmp(card.suit, drawDeck[i].suit))
        {
        if(!strcmp(card.number, drawDeck[i].number))
        {
            for(int j = i; j < cardsInDrawDeck; j++)
            {
                drawDeck[j] = drawDeck[j+1];
            }
            cardsInDrawDeck--;
            break;
        }
        }
    }
}

void putCardInPlayersHand(int round, Card card)
{
    Players[round].cardsInHand[Players[round].numOfCards] = card;
    Players[round].numOfCards++;
    Players[round].sumOfCards += card.value;
    
    if(Players[round].sumOfCards > 21)
    {
        if(!Players[round].playerstate.hasAInHand)
        {
            //maybe he has - checking
            for(int i = 0; i < Players[round].numOfCards; i++)
            {
                if(!strcmp(Players[round].cardsInHand[i].number, "A"))
                {
                    Players[round].sumOfCards -= 10;
                    Players[round].playerstate.hasAInHand = 1;
                }
            }
            //he does not have - busted
            if(!Players[round].playerstate.hasAInHand)
            {
                showPlayersHand(round);
                printf("BUSTED!\n\n");
                Players[round].playerstate.isBust = 1;
            }
        }else
        {
            showPlayersHand(round);
            printf("BUSTED!\n\n");
            Players[round].playerstate.isBust = 1;
        }
    }
    else if(21 == Players[round].sumOfCards)
    {
        if(2 == Players[round].numOfCards)
        //to be tested better
        // if(state = dealFirstHand)
        {
            Players[round].playerstate.BLACKJACK = 1;
        }
    }
}

Card dealCard(void)
{
    int pos = rand() % cardsInDrawDeck;
    deleteFromDeck(drawDeck[pos]);
    return drawDeck[pos];
}

void handleThisPlayer(int playerNo)
{
    char a = ' ';

    while (!(!Players[playerNo].playerstate.isStaying) 
          ^(!Players[playerNo].playerstate.isBust))
    {
        /* code */
        showPlayersHand(playerNo);
        printf("apasati h pentru hit sau s pentru stay: ");
        scanf(" %c", &a);
        // a = 'h';
        getchar();
        printf("\n");
        switch (a)
        {
        case 'h':
            /* code */
            putCardInPlayersHand(playerNo, dealCard());
            break;
        
        case 's':
            /* code */
            printf("OK\n");
            Players[playerNo].playerstate.isStaying = 1;
            break;

        default:
            printf("\nWhoops\n");
            break;
        }
    }
}

void showPlayersHand(int playerNo)
{
    printf("jucatorul %s are in mana urmatoarele carti cu o suma de %d: ", Players[playerNo].name, Players[playerNo].sumOfCards);

    for(int i = 0; i < Players[playerNo].numOfCards; i++) 
    {
        printf("%s%s ", Players[playerNo].cardsInHand[i].number, Players[playerNo].cardsInHand[i].suit);
    }

    printf("\n");
}

void handlePlayerCredit(int playerNo)
{
    //non-efficient way of doing things, but pretty ok
    if(Players[playerNo].sumOfCards > Players[0].sumOfCards && 
      !Players[playerNo].playerstate.isBust && 
      !Players[playerNo].playerstate.BLACKJACK)
    {
        Players[playerNo].playerstate.isWinner = 1;
    }

    if(Players[playerNo].playerstate.isBust)
    {
        Players[playerNo].credit -= Players[playerNo].bidValue;
    }else if(Players[playerNo].playerstate.isWinner)
    {
        Players[playerNo].credit += Players[playerNo].bidValue;
    }else if(Players[playerNo].playerstate.BLACKJACK)
    {
         Players[playerNo].credit += (Players[playerNo].bidValue*1.5);
    }
}
//////////////////////////////// IMPLEMENTATION ////////////////////////////////