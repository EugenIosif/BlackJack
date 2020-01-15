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
    hitPlayer,
    stayPlayer, 
    blackjack,
    //add more gamestates
}States;

typedef struct Player
{
    char name[10];
    Card cardsInHand[5];
    unsigned short int numOfCards;
    unsigned short int sumOfCards;
    struct playerState {
        unsigned char isStaying : 1;
        unsigned char isDead : 1;
        unsigned char isWinner : 1;
        unsigned char _blank : 1;
        unsigned char _reserved : 4;
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
    Players[0] = generatePlayer(0);
    printf("%s\n", Players[0].name);
}

int main(void)
{
    
    initFunction();
    testFunction();
    char a = ' ';

    while(state != gameOver)
    {
        switch (state)
        {
        case startGame:
            /* code */
            printf("introduceti s pentru a incepe jocul sau q pentru a-l parasi: \n");
            scanf("%c", &a);
            //printf("\n");

            if('s' == a)
            {
                printf("introduceti numarul de jucatori: ");
                scanf("%d", &numberOfPlayers);
                printf("\n");

                for(int i = 0; i < numberOfPlayers; i++)
                {
                    Players[i] = generatePlayer(i);
                }
                
                state = dealFirstHand;
            }
            else if('q' == a)
            {
                state = quitGame;
            }
            else
            {
                printf("WRONG!\n");
                state = startGame;
            }
            
            break;

        case quitGame:
            /* code */
            break;
        
        case dealFirstHand:
            /* code */
            break;

        case blackjack:
            /* code */
            break;

        case hitPlayer:
            /* code */
            break;

        case stayPlayer:
            /* code */
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
    player.playerstate.isDead = 0;
    player.playerstate.isStaying = 0;
    player.playerstate.isWinner = 0;
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
        for(int i = 0; i < Players[round].numOfCards; i++)
        {
            if(!strcmp(Players[round].cardsInHand[i].number, "A"))
            {
                Players[round].sumOfCards -= 10;
            }
            else
            {            
                Players[round].playerstate.isDead = 1;
            }
        }
    }
    else if(21 == Players[round].sumOfCards)
    {
        if(2 == Players[round].numOfCards)
        {
            Players[round].playerstate.isWinner = 1;
            Players[round].playerstate.isStaying = 1;
            state = blackjack;
        }
    }
    
}

Card dealCard(void)
{
    int pos = rand() % cardsInDrawDeck;
    printf("deleting the %s%s\n", drawDeck[pos].number, drawDeck[pos].suit);
    deleteFromDeck(drawDeck[pos]);
    return drawDeck[pos];
}

//////////////////////////////// IMPLEMENTATION ////////////////////////////////