////  MTA taki game
// Name: Gal Melman
// ID- 319041950

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#define COLOR_GREEN 'G'
#define COLOR_BLUE 'B'
#define COLOR_YELLOW 'Y'
#define COLOR_RED 'R'
#define NO_COLOR "NONE"
#define ONE_CARD '1'
#define TWO_CARD '2'
#define THREE_CARD '3'
#define FOUR_CARD '4'
#define FIVE_CARD '5'
#define SIX_CARD  '6'
#define SEVEN_CARD '7'
#define EIGHT_CARD '8'
#define NINE_CARD '9'
#define PLUS_CARD  '+'
#define TAKI_CARD  "TAKI"
#define CHANGE_COLOR_CARD  "COLOR"
#define DEFAULT_COLOR_FOR_CHANGE_COLOR_CARD '0'
#define STOP_CARD  "STOP"
#define CHANGE_DIRECTION_CARD  "<->"
#define NAME_SIZE  20
#define POSITIVE_DIRECTION 1
#define NEGATIVE_DIRECTION -1
#define BOARD_CARD "CARD" 
#define MAX_CARDS 56
#define MAX_STRING_LENGTH 6
#define STARTING_NUMBER_OF_CARDS 4
#define FRAME_WIDTH 8
#define TYPES_OF_CARDS 14
#define LAST_NORMAL_CARD 9
#define ZEROED 0
#define WIDTH_COUNT 8


typedef struct CARDS_STATS {
	char type[MAX_STRING_LENGTH];
	int count;

}CARDS_STATS;



typedef struct CARD {
	char number[MAX_STRING_LENGTH];
	char Color;

}CARD;

typedef struct PLAYERINFO {
	char name[NAME_SIZE];
	struct CARD* Cards;
	int Num_cards;
	int Array_size;
}PLAYERINFO;


CARD* initializeDeck(); // intilize a deck of cards
void initializeCards(PLAYERINFO* newplayer);
CARD* Deal(PLAYERINFO* players, int numOfPlayers, CARDS_STATS* cardsStatistics); // deal 4 cards for each player
void readPlayerNames(PLAYERINFO* newplayer, int numOfPlayer);
PLAYERINFO* readPlayersData(int* numOfPlayers);
CARD* GetUpperCard(CARD* deckAdress, CARDS_STATS* cardsStatistics);
void printCard(CARD* card);
void printUppercard(CARD* uppercard);
void showPlayerCards(PLAYERINFO* player);
int getNextPlayer(int currentPlayer, int direction, int numOfPlayers);
int takePlayerChoice(PLAYERINFO* player, bool* isTakiCardActive);
void reallocateCardArray(PLAYERINFO* player);
void removeCardFromDeck(PLAYERINFO* player, int cardIndex);
void draw(PLAYERINFO* playerdata, CARD* deckOfCards, CARDS_STATS* cardsStatistics);
int chooseColor();
bool isCardValidPick(PLAYERINFO* playerdata, CARD* upperCard, int* playersChoise, bool* takCardActive);
void handleSpecialCard(PLAYERINFO* player, CARD* upperCard, CARD* deck, int choice, int* direction, int* playerIndex, int numOfPlayers, bool* takiCardActive, CARDS_STATS* cardsStatistics);
bool playTurn(PLAYERINFO* player, CARD* upperCard, CARD* deck, int* direction, int* playerIndex, int numOfPlayers, bool* takiCardActive, CARDS_STATS* cardsStatistics);
bool isWinner(PLAYERINFO* player);
void freeMemory(PLAYERINFO* players, int numOfPlayers, CARD* deck);
CARDS_STATS* initializeCardsStatsArray();
void addCardToStatistics(CARD* card, CARDS_STATS* cardsStatistics);
void printCardStatistics(CARDS_STATS* cardsStatistics);

int main()
{
	PLAYERINFO* players;
	CARD* deck;
	CARD* upperCard;
	CARDS_STATS* cardsStatistics;
	int numOfPlayers;
	int playerindex = 0;
	int direction = POSITIVE_DIRECTION;
	int numCardsInDeck = MAX_CARDS;
	bool noWinner = false;
	bool validPick = true;
	bool takiCardActive = false;

	printf("************  Welcome to TAKI game !!! ***********\n");
	;
	cardsStatistics = initializeCardsStatsArray(); // sets the arrey with all the cards types and thier coounters set to 0.
	players = readPlayersData(&numOfPlayers); //get the adress for the players data.
	deck = Deal(players, numOfPlayers, cardsStatistics); // deal 4 cards
	upperCard = GetUpperCard(deck, cardsStatistics); //get upper card adress. 



	//gameloop
	while (!noWinner)
	{
		//play turn for cuurent playerindex.
		playTurn(&players[playerindex], upperCard, deck, &direction, &playerindex, numOfPlayers, &takiCardActive, cardsStatistics);
		//// Check if there is a winner
		if (noWinner = isWinner(&players[playerindex])) {
			printf("\nThe winner is %s! Congratulations!\n", players[playerindex].name);
			continue;
		}
		//if the taki card is not active end the turn and get the next player index
		if (!takiCardActive)
		{
			playerindex = getNextPlayer(playerindex, direction, numOfPlayers); // get the next player index

		}
		// the taki card is active keep the turn for the current player.
		else {
			continue;
		}



	}

	printCardStatistics(cardsStatistics); // print the game statistics.
	freeMemory(players, numOfPlayers, deck); // free the memory allocated during the game
	return 0;
}

PLAYERINFO* readPlayersData(int* numOfPlayers)   //gather the number of players participating in the game and allocate memory for that many players and collect their names.
// the function returns the adress of the players data.
{
	bool validNUmberOfPlayers = false;

	PLAYERINFO* players = NULL;
	while (!validNUmberOfPlayers) // making sure the number of players is a valid number.
	{
		printf("Please enter the number of players: \n");
		scanf("%d", numOfPlayers);
		if (*numOfPlayers <= 0) {
			printf("unvalid number of players try again! \n");
			continue;
		}
		validNUmberOfPlayers = true;

	}
	players = (PLAYERINFO*)malloc(*numOfPlayers * sizeof(PLAYERINFO)); //set the structs by number of player
	if (!players) // check if alloction failed
	{
		printf("error!  alloction failed");
		exit(1); //exit program
	}
	for (int i = 0; i < *numOfPlayers; i++)
	{
		readPlayerNames(players + i, i); // collects the name for each player
		initializeCards(players + i);

	}

	return players;
}

void readPlayerNames(PLAYERINFO* newplayer, int numOfPlayer) // A function that gets the player name and update the playerinfo.
{
	printf("Please enter the first name of player #%d:\n", (numOfPlayer + 1));
	scanf("%s", newplayer->name);

}

void initializeCards(PLAYERINFO* newplayer)
{// // Allocate memory for the player's cards
	newplayer->Cards = (CARD*)malloc(STARTING_NUMBER_OF_CARDS * sizeof(CARD));
	if (!newplayer->Cards) // check if alloction failed
	{
		printf("error!  alloction failed");
		exit(1); //exit program
	}
	newplayer->Num_cards = STARTING_NUMBER_OF_CARDS; // The player has 4 cards initially
	newplayer->Array_size = STARTING_NUMBER_OF_CARDS; // The maximum number of cards the player can hold is 4

}

CARD* Deal(PLAYERINFO* players, int numOfPlayers, CARDS_STATS* cardsStatistics) 
// deal 4 cards for each player.
// returns the adress of the card deck.
{
	CARD* initializedDeck = initializeDeck(); // Create the deck of cards
	srand((unsigned int)time(NULL)); // Seed the random number generator

	for (int i = 0; i < numOfPlayers; i++) {
		for (int j = 0; j < STARTING_NUMBER_OF_CARDS; j++) {
			int randomIndex = rand() % MAX_CARDS; // Get a random index for the remaining cards in the deck
			players[i].Cards[j] = initializedDeck[randomIndex]; // Give the player the card at that index
			addCardToStatistics(&players[i].Cards[j], cardsStatistics);
		}
	}
	return initializedDeck; // return the remaining cards in the deck

}

CARD* initializeDeck() // a function that initlize a deck of cards.
// the function return a output the adress of the deck of cards.
{
	CARD* deck = NULL;
	deck = (CARD*)malloc(MAX_CARDS * sizeof(CARD)); //set the structs by number of cards 
	if (!deck) // check if alloction failed
	{
		printf("error!  alloction failed");
		exit(1); //exit program
	}
	int index = 0; //represent the index in the deck 
	// Initialize the cards with numbers and colors
	for (int i = 0; i < 9; i++) {
		deck[index].number[0] = i + ONE_CARD; //get the next ascii value + '1'
		deck[index].number[1] = '\0';
		deck[index].Color = COLOR_GREEN;
		index++;
		deck[index].number[0] = i + ONE_CARD;
		deck[index].number[1] = '\0';
		deck[index].Color = COLOR_RED;
		index++;
		deck[index].number[0] = i + ONE_CARD;
		deck[index].number[1] = '\0';
		deck[index].Color = COLOR_YELLOW;
		index++;
		deck[index].number[0] = i + ONE_CARD;
		deck[index].number[1] = '\0';
		deck[index].Color = COLOR_BLUE;
		index++;
	}

	// Initialize the "+" cards
	deck[index].number[0] = PLUS_CARD;
	deck[index].number[1] = '\0';
	deck[index].Color = COLOR_GREEN;
	index++;
	deck[index].number[0] = PLUS_CARD;
	deck[index].number[1] = '\0';
	deck[index].Color = COLOR_RED;
	index++;
	deck[index].number[0] = PLUS_CARD;
	deck[index].number[1] = '\0';
	deck[index].Color = COLOR_YELLOW;
	index++;
	deck[index].number[0] = PLUS_CARD;
	deck[index].number[1] = '\0';
	deck[index].Color = COLOR_BLUE;
	index++;


	// Initialize the TAKI cards
	strcpy(deck[index].number, TAKI_CARD);
	deck[index].Color = COLOR_GREEN;
	index++;
	strcpy(deck[index].number, TAKI_CARD);
	deck[index].Color = COLOR_RED;
	index++;
	strcpy(deck[index].number, TAKI_CARD);
	deck[index].Color = COLOR_YELLOW;
	index++;
	strcpy(deck[index].number, TAKI_CARD);
	deck[index].Color = COLOR_BLUE;
	index++;


	// Initialize the CHANGE DIRECTION cards
	strcpy(deck[index].number, CHANGE_DIRECTION_CARD);
	deck[index].Color = COLOR_GREEN;
	index++;
	strcpy(deck[index].number, CHANGE_DIRECTION_CARD);
	deck[index].Color = COLOR_RED;
	index++;
	strcpy(deck[index].number, CHANGE_DIRECTION_CARD);
	deck[index].Color = COLOR_YELLOW;
	index++;
	strcpy(deck[index].number, CHANGE_DIRECTION_CARD);
	deck[index].Color = COLOR_BLUE;
	index++;

	// Initialize the STOP cards
	strcpy(deck[index].number, STOP_CARD);
	deck[index].Color = COLOR_GREEN;
	index++;
	strcpy(deck[index].number, STOP_CARD);
	deck[index].Color = COLOR_RED;
	index++;
	strcpy(deck[index].number, STOP_CARD);
	deck[index].Color = COLOR_YELLOW;
	index++;
	strcpy(deck[index].number, STOP_CARD);
	deck[index].Color = COLOR_BLUE;
	index++;

	// Initialize the CHANGE COLOR cards
	strcpy(deck[index].number, CHANGE_COLOR_CARD);
	deck[index].Color = DEFAULT_COLOR_FOR_CHANGE_COLOR_CARD;
	index++;
	strcpy(deck[index].number, CHANGE_COLOR_CARD);
	deck[index].Color = DEFAULT_COLOR_FOR_CHANGE_COLOR_CARD;
	index++;
	strcpy(deck[index].number, CHANGE_COLOR_CARD);
	deck[index].Color = DEFAULT_COLOR_FOR_CHANGE_COLOR_CARD;
	index++;
	strcpy(deck[index].number, CHANGE_COLOR_CARD);
	deck[index].Color = DEFAULT_COLOR_FOR_CHANGE_COLOR_CARD;


	return deck;

}

CARD* GetUpperCard(CARD* deckAdress, CARDS_STATS* cardsStatistics)
{  //Get first card for  the game
 
	CARD* upperCard = deckAdress; 
	int randomIndex = rand() % MAX_CARDS; // Get a random index for the  cards in the deck
	while (deckAdress[randomIndex].number[0] < ONE_CARD || (deckAdress[randomIndex].number[0] > NINE_CARD))
	{// While not equal to 1 - 9, get again
		randomIndex = rand() % MAX_CARDS;
		continue;

	}
	*upperCard = deckAdress[randomIndex]; //sets the uppercard adress to be the picked card
	addCardToStatistics(upperCard, cardsStatistics);
	return upperCard; //return the adress of the card.
}

void printUppercard(CARD* uppercard)
{ // a function that prints the uppercard.
	printf("\nUpper card: \n");
	printCard(uppercard);

}

void printCard(CARD* card) {//a function that prints a card.
	// Get the length of the card's number
	int len = (int)strlen(card->number);
	// Print the top of the frame
	printf("*********\n");
	// Print the first line of the frame
	printf("*");
	// Print spaces before the number to center it
	for (int i = 0; i < (FRAME_WIDTH - len) / 2; i++) {
		printf(" ");
	}
	// Print the card's number
	printf("%s", card->number);
	// Print spaces after the number to center it
	for (int i = 1; i < (FRAME_WIDTH - len) / 2; i++) {
		printf(" ");
	}
	// If the length of the number is odd, print an extra space to keep the frame aligned
	if ((FRAME_WIDTH - len) % 2) printf(" ");
	printf("*\n");
	// Print the third line of the frame
	if ((card->Color == DEFAULT_COLOR_FOR_CHANGE_COLOR_CARD))
	{
		printf("*       *\n"); // if the card is change color print the card witout the card color so the pplayer can choose it.
	}
	else
	{
		printf("*   %c   *\n", card->Color);
		// Print the forth line of the frame
	}
	printf("*       *\n");
	// Print the bottom of the frame
	printf("*********\n");
}

void showPlayerCards(PLAYERINFO* player)
{//A function that prints the player cards
	for (int i = 0; i < player->Num_cards; i++)
	{
		printf("\nCard #%d: \n", i + 1);
		printCard(player->Cards + i);
	}

}

int getNextPlayer(int currentPlayer, int direction, int numOfPlayers)
{ // Add the current player and the direction of play
	// and take the result modulo the number of players 
	if (currentPlayer == 0 && direction == NEGATIVE_DIRECTION) // making sure the player index wont be a negative value.
	{
		return (numOfPlayers - 1); // go to the last player in line.
	}
	else {
		return ((currentPlayer + direction) % numOfPlayers);
	} // returns the next player index]
}

int takePlayerChoice(PLAYERINFO* player, bool* isTakiCardActive)
{ //A function that gets a input the player choice for the next action ( card or take card from deck)
	// the funcrion returns the  player choice.
	int playerChoice;
	bool validChoise = false;
	while (!validChoise)
	{
		if (*isTakiCardActive)
		{ // if the taki card is active :
			printf("\nPlease enter 0 if you want to finish your turn\n");
			printf("or 1-%d if you want to put one of your cards in the middle: \n", player->Num_cards);
			scanf("%d", &playerChoice);
		}
		else {
			//if the taki card isn't active:
			printf("\nPlease enter 0 if you want to take a card from the deck\n");
			printf("or 1-%d if you want to put one of your cards in the middle: \n", player->Num_cards);
			scanf("%d", &playerChoice);

		}
		if (!(validChoise = (playerChoice >= 0 && playerChoice <= player->Num_cards))) {
			printf("\nInvalid choice! Try again."); // if the player pick is unvalid.
		}

	}

	return playerChoice;
}

void draw(PLAYERINFO* playerdata, CARD* deckOfCards, CARDS_STATS* cardsStatistics) {
	// A function that draw  new card for the current player and update the Statistics.
	int cardIndex = playerdata->Num_cards; // sets the card index to the number of cards currently held by the player
	if (playerdata->Num_cards == playerdata->Array_size) // checks if the num of card equals the array of card size
	{
		reallocateCardArray(playerdata); // reallocate the array size .
	}
	int randomIndex = rand() % MAX_CARDS; // Get a random index for the  cards in the deck
	CARD newCard = deckOfCards[randomIndex]; //sets the uppercard adress to be the picked card
	addCardToStatistics(&newCard, cardsStatistics);    // adds the drawn card to the statistics
	playerdata->Cards[playerdata->Num_cards] = newCard; // add the new card to the player's cards
	playerdata->Num_cards++;// increment the number of cards held by the player by one

}

bool isCardValidPick(PLAYERINFO* playerdata, CARD* upperCard, int* playersChoise, bool* takCardActive)
{ // A function thats check if the chosen card is equal to the uppercard in number or color or if the chosen card is change color card
	// the function returns a bool value .

	// Decrement the player choice to match the array index 
	(*playersChoise)--;
	// Initialize a variable to store the validity of the pick
	bool isValidPick = false;
	// Check if the chosen card is equal to the upper card in number or color while taki card isn't active.
	if (((playerdata->Cards[*playersChoise].Color == upperCard->Color) || (strcmp(playerdata->Cards[*playersChoise].number, upperCard->number) == 0)) && (!(*takCardActive)))
	{
		isValidPick = true;
	}
	else if (playerdata->Cards[*playersChoise].Color == upperCard->Color) // if the taki card is active and the card match in color.
	{
		isValidPick = true;

	}
	// check if the chosen card is a change color card
	else if ((strcmp(playerdata->Cards[*playersChoise].number, CHANGE_COLOR_CARD) == 0))
	{
		isValidPick = true;
	}
	// return the validity of the chosen card
	return isValidPick;
}

void handleSpecialCard(PLAYERINFO* player, CARD* upperCard, CARD* deck, int choice, int* direction, int* playerIndex, int numOfPlayers, bool* takiCardActive, CARDS_STATS* cardsStatistics) {
	
	
	// Handle the "STOP" card
	if (strcmp(player->Cards[choice].number, STOP_CARD) == 0) {
		// Pass turn to next player
		if (!(*takiCardActive))
		{
			if (*direction == NEGATIVE_DIRECTION)
			{
				(*playerIndex)--;  // if the direction is negative decrement by 1 the playerindex so the next player will be skipped.
			}
			else
			{
				(*playerIndex)++; //only if the taki card is not active increment the playerindex by 1 so the next player will be skipped.
			}
		}
		*upperCard = player->Cards[choice]; //set the upper card to be the chosen card.
		removeCardFromDeck(player, choice); 
		if (player->Num_cards == 0 && numOfPlayers >= 2) // if there are 2 or more players and the players choose stop as his last card, new card is given .
		{
			draw(player, deck, cardsStatistics);
		}
		return;
	}
	// Handle the "PLUS" card
	else if (player->Cards[choice].number[0] == PLUS_CARD) {
		*upperCard = player->Cards[choice]; //set the upper card to be the chosen card.
		removeCardFromDeck(player, choice);
		if (*direction == NEGATIVE_DIRECTION)
		{
			(*playerIndex)++; // decrement the player index by 1 so the turn will stay for the current player index.

		}
		else
		{
			(*playerIndex)--; // decrement the player index by 1 so the turn will stay for the current player index.
		}
		if (player->Num_cards == 0) // if the player chose plus as his last card give him another card witout asking.
		{
			draw(player, deck, cardsStatistics);

		}
		return;
	}
	// Handle the "TAKI" card
	else if (strcmp(player->Cards[choice].number, TAKI_CARD) == 0) {
		// Keep playing current player's turn until a different card is played or the player has no cards left
		*upperCard = player->Cards[choice];
		removeCardFromDeck(player, choice);
		(*takiCardActive) = true;
		return;
	}
	// Handle the "CHANGE COLOR" card
	else if (strcmp(player->Cards[choice].number, CHANGE_COLOR_CARD) == 0)
	{
		// Ask player to choose a new color
		int colorPicked = chooseColor();
		while (colorPicked > 4 || colorPicked < 1) // if the player choose an invalid color the program asks again.
		{
			printf("\nInvalid color! Try again.\n");
			colorPicked = chooseColor();

		}
		switch (colorPicked)
		{ // update the card color with the picked color
		case 1:
			player->Cards[choice].Color = COLOR_YELLOW;
			break;
		case 2:
			player->Cards[choice].Color = COLOR_RED;
			break;
		case 3:
			player->Cards[choice].Color = COLOR_BLUE;
			break;
		case 4:
			player->Cards[choice].Color = COLOR_GREEN;
			break;
		default:
			break;
		}
		*upperCard = player->Cards[choice]; //update the uppercard to be the change color card with the updated color
		removeCardFromDeck(player, choice);
		if (takiCardActive) // if the taki card is active and a change color is picked , end the taki flow.
		{
			(*takiCardActive) = false;
		}
		return;


	}
	// Handle the "CHANGE DIRECTION" card
	else if (strcmp(player->Cards[choice].number, CHANGE_DIRECTION_CARD) == 0) {
		// Change the direction of play
		if (!(*takiCardActive)) // while the taki card isnt active and the card is not in the middle of cards flow.
		{
			(*direction) *= NEGATIVE_DIRECTION; //change the play direction 
		}
		*upperCard = player->Cards[choice];
		removeCardFromDeck(player, choice);
		return;
	}
	return;
}
 
int chooseColor()
{ // A function that gets as input the color chosen from the player for change color card.
	//returns the chosen color as integer
	int colorPicked;
	printf("Please enter your color choice: \n"
		"1 - Yellow\n"
		"2 - Red\n"
		"3 - Blue\n"
		"4 - Green\n");
	scanf("%d", &colorPicked);
	return colorPicked;
}

void reallocateCardArray(PLAYERINFO* player)
{//A function to that reallocate memory for a player's array of cards
	CARD* temp;


	player->Array_size *= 2; // double the size of the array.
	temp = (CARD*)realloc(player->Cards, player->Array_size * sizeof(CARD)); //allocate the new arrey.
	if (!temp) // check if alloction failed
	{
		printf("error!  alloction failed");
		exit(1); //exit program
	}
	player->Cards = temp; // if realloc successful pass the adress to the players deck of cards.

}

void removeCardFromDeck(PLAYERINFO* player, int choice)
{ // A function that recives as input the player data and the player choice of card.
	//the function remove the picked card and rearrange the deck of cards witout the chosen card.
	for (int i = choice; i < player->Num_cards - 1; i++) { //for every card left to the chosen card.
		player->Cards[i] = player->Cards[i + 1]; // move every card from the chosen card to the left
	}
	player->Num_cards -= 1; // decrement the number of card in the deck.

}

bool playTurn(PLAYERINFO* player, CARD* upperCard, CARD* deck, int* direction, int* playerIndex, int numOfPlayers, bool* takiCardActive, CARDS_STATS* cardsStatistics)
{ //This function is used to simulate a turn in a taki game. 
	bool isValidChoise = false;
	printUppercard(upperCard);//print the uppercard
	printf("\n%s's turn\n", player->name); // prints the player's name.
	// Print player's cards
	showPlayerCards(player);
	while (!isValidChoise)
	{
		// Get player's choice
		int choice = takePlayerChoice(player, takiCardActive);
		// Check if the player chose to draw a card from the deck.
		if (choice == 0 && (!(*takiCardActive)))
		{
			draw(player, deck, cardsStatistics);
			return true;
		}
		else if (choice == 0 && (*takiCardActive)) // if the player choose to end the taki flow and pass the turn to the next player.
		{
			(*takiCardActive) = false;
			return true;
		}
		else if (isValidChoise = (isCardValidPick(player, upperCard, (&choice), takiCardActive)))
		{
			if (!((player->Cards[choice].number[0] > PLUS_CARD) && (player->Cards[choice].number[0] <= NINE_CARD))) // if the card is special card.
			{
				handleSpecialCard(player, upperCard, deck, choice, direction, playerIndex, numOfPlayers, takiCardActive, cardsStatistics);
			}
			else //the card is  a normal card
			{
				*upperCard = player->Cards[choice];
				// remove the card from the player's hand
				removeCardFromDeck(player, choice);
				return true;
			}

		}
		else
		{
			printf("Invalid card! Try again.\n"); // if the player chose an unvalid choise the loop runs again.
		}

	}

	return false;
}

bool isWinner(PLAYERINFO* player) {
	// function that checks if the player has won the game
   // by checking if the player has no cards left in their hand
	if (player->Num_cards == 0) {
		return true;
	}
	return false;
}

void freeMemory(PLAYERINFO* players, int numOfPlayers, CARD* deck) {
	// a function that free the memory allocated during the game.
	int i;
	// Iterate through all players
	for (i = 0; i < numOfPlayers; i++) {
		// Check if the player has any cards
		if (players[i].Cards) {
			// Free the memory allocated for the player's cards
			free(players[i].Cards);
			// set the pointer to NULL
			players[i].Cards = NULL;
		}
	}
	// check if the deck of cards is not null
	if (deck) {
		// Free the memory allocated for the deck of cards
		free(deck);
		// set the pointer to NULL
		deck = NULL;
	}
	// check if the players array is not null
	if (players) {
		// Free the memory allocated for the array of playerInfo structs
		free(players);
		// set the pointer to NULL
		players = NULL;
	}
	return;
}

CARDS_STATS* initializeCardsStatsArray()
{ //function is responsible for allocating memory for an array of CARDS_STATS structs
	//and initializing the values of the structs with the different card types and their counts set to 0.
	// it returns the pointer to the array of CARDS_STATS structs,
	//which can be used to access the statistics of the different card types in the game.
	int specialCardIndex = LAST_NORMAL_CARD;
	CARDS_STATS* cardsStatistics = NULL;
	cardsStatistics = (CARDS_STATS*)malloc(15 * sizeof(CARDS_STATS)); //set the structs by number of cards 
	if (!cardsStatistics) // check if alloction failed
	{
		printf("error!  alloction failed");
		exit(1); //exit program
	}
	// Initialize the card types with numbers
	for (int i = 0; i < LAST_NORMAL_CARD; i++)
	{
		cardsStatistics[i].type[0] = i + ONE_CARD; //get the next ascii value + '1'
		cardsStatistics[i].type[1] = '\0'; //set a terminator
		cardsStatistics[i].count = ZEROED;
	}
	// Initialize the PLUS card
	cardsStatistics[specialCardIndex].type[0] = PLUS_CARD; //get the next ascii value + '1'
	cardsStatistics[specialCardIndex].type[1] = '\0'; //set a terminator
	cardsStatistics[specialCardIndex].count = ZEROED;
	specialCardIndex++;

	// Initialize the TAKI card
	strcpy(cardsStatistics[specialCardIndex].type, TAKI_CARD);
	cardsStatistics[specialCardIndex].count = ZEROED;
	specialCardIndex++;

	// Initialize the CHANGE COLOR card
	strcpy(cardsStatistics[specialCardIndex].type, CHANGE_COLOR_CARD);
	cardsStatistics[specialCardIndex].count = ZEROED;
	specialCardIndex++;

	// Initialize the CHANGE DIRECTION card
	strcpy(cardsStatistics[specialCardIndex].type, CHANGE_DIRECTION_CARD);
	cardsStatistics[specialCardIndex].count = ZEROED;
	specialCardIndex++;

	// Initialize the STOP card
	strcpy(cardsStatistics[specialCardIndex].type, STOP_CARD);
	cardsStatistics[specialCardIndex].count = ZEROED;
	specialCardIndex++;

	return cardsStatistics;
}

void addCardToStatistics(CARD* card, CARDS_STATS* cardsStatistics)
{   //a function that adds the cuurent card to the statistics.
	// checks for  a match between the card and the numbers type card and update their counter.
	int specialCardIndex = LAST_NORMAL_CARD;
	for (int i = 0; i < LAST_NORMAL_CARD; i++)
	{
		if (card->number[ZEROED] == (ONE_CARD + i))
			cardsStatistics[i].count++;
	}
	//check for plus card
	if (card->number[ZEROED] == PLUS_CARD)
	{
		cardsStatistics[specialCardIndex].count++;

	}
	specialCardIndex++;
	//check for taki card
	if (strcmp(card->number, TAKI_CARD) == 0)
	{
		cardsStatistics[specialCardIndex].count++;

	}
	specialCardIndex++;
	//check for CHANGE COLOR
	if (strcmp(card->number, CHANGE_COLOR_CARD) == 0)
	{
		cardsStatistics[specialCardIndex].count++;

	}
	specialCardIndex++;
	//check for CHANGE DIRECTION card
	if (strcmp(card->number, CHANGE_DIRECTION_CARD) == 0)
	{
		cardsStatistics[specialCardIndex].count++;

	}
	specialCardIndex++;
	//check for stop card
	if (strcmp(card->number, STOP_CARD) == 0)
	{
		cardsStatistics[specialCardIndex].count++;
	}


}

void printCardStatistics(CARDS_STATS* cardsStatistics)
{ // A function that sort the cardsStatistics array and prints the updated array.
	int i, j;
	// use bubble sort to sort the array of CARDS_STATS structs based on their counts
	for (i = 0; i < TYPES_OF_CARDS ; i++)
	{
		for (j = 0; j < TYPES_OF_CARDS - i ; j++)
		{
			if (cardsStatistics[j].count < cardsStatistics[j + 1].count)
			{
				CARDS_STATS temp = cardsStatistics[j]; //swap the two structs
				cardsStatistics[j] = cardsStatistics[j + 1];
				cardsStatistics[j + 1] = temp;
			}
		}
	}
	// print the sorted statistics
	printf("\n************ Game Statistics ************ \n");
	printf("Card #   |   Freaquency\n");
	printf("----------------------------\n");
	for (i = 0; i < TYPES_OF_CARDS ; i++)
	{
		printf("%-*s   %c %*d\n", MAX_STRING_LENGTH, cardsStatistics[i].type, '|', WIDTH_COUNT, cardsStatistics[i].count);
	}
}

