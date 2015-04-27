/*
 * boggleplay.cpp is the client used to output
 * to the user interface and to also 
 * interact with the Boggle class for
 * information about the game
 */

#include <string>
#include <cctype>
#include "lexicon.h"
#include "simpio.h"
#include "Boggle.h"
#include "strlib.h"
#include "console.h";

//Prototypes
string getUserBoard();
bool isValidBoardText(string boardText);
void playTurn(Boggle& boggle, Player player, string word);
void printTurn(Boggle &boggle, Player player);

/*
 * Plays a game of Boggle and prints out
 * directions to the user. It continues until
 * the user opts out of playing. It leverages
 * the Boggle class in helper functions that
 * carry out the explicit actions of the game
 * Parameters:
 * - Dictionary used to check words in game
 */
void playOneGame(Lexicon& dictionary) {
	string boardText;
	if(!getYesOrNo("Do you want to generate a random board? ")) boardText = getUserBoard();
    clearConsole();
	Boggle boggle(dictionary, boardText);
	cout << "It's your turn!" << endl;
	printTurn(boggle, HUMAN);
	while(true) {
        string word;
		word = toUpperCase(getLine("Type a word (or Enter to stop): "));
		if(word.empty()) break;
		playTurn(boggle, HUMAN, word);
	}
	printTurn(boggle, COMPUTER);
}

/*
 * Prompts the user to input a custom string of
 * Boggle cubes if they don't elect a random board
 * to play the game
 */
string getUserBoard() {
	string boardText;
	while(true) {
		boardText = getLine("Type the 16 letters to appear on the board: ");
		if(isValidBoardText(boardText)) break;
		cout << "That is not a valid 16-letter board string. Try again." << endl;
	}
	boardText = toUpperCase(boardText);
	return boardText;
}

/*
 * Helper fucntion for getUserBoard() that verifys
 * if the user's custom Boggle board is appropriate,
 * checking for length and all letter entries
 */
bool isValidBoardText(string boardText) {
    if(boardText.length() != Boggle::kBoardSize) return false;
	istringstream stream(boardText);
	char ch;
	while(stream >> ch) {
		if(!isalpha(ch)) return false;
	}
	return true;
}

/*
 * Given a user's inputted Boggle word, this procedure
 * checks whether the word is valid, is contained in the 
 * board, and prints out all corresponding updates of the
 * game based on whether the word is valid
 */
void playTurn(Boggle& boggle, Player player, string word) {
	clearConsole();
	if(boggle.checkWord(word)) {
		if(boggle.humanWordSearch(word)) {
			cout << "You found a new word! \"" << word << "\"" << endl;	
		} else {
			cout << "That word can\'t be formed on this board." << endl;
		}
	} else {
		cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
	}
    printTurn(boggle, player);
}

/*
 * Returns the corresponding game updates for the game player
 * that made the call. Because outputs differ for the human and
 * computer, this function delineates between the appropriate
 * console values.
 */
void printTurn(Boggle& boggle, Player player) {
	int humanScore;
	int computerScore;
	if(player == HUMAN) {
		cout << boggle << endl;
	} else {
		cout << endl << "It's my turn!" << endl;
	}
	cout << ((player == HUMAN) ? "Your" : "My") << " words (";
	cout << ((player == HUMAN) ? boggle.humanWordSet().size() : boggle.computerWordSearch().size()) << "): ";
    cout << ((player == HUMAN) ? boggle.humanWordSet() : boggle.computerWordSearch()) << endl;
    cout << ((player == HUMAN) ? "Your" : "My") << " score: ";
    cout << ((player == HUMAN) ? (humanScore = boggle.humanScore()) : (computerScore = boggle.getScoreComputer()));
    cout << endl;

	if(player == COMPUTER) {
		if(computerScore > humanScore) {
			cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
		} else {
			cout << "WOW, you defeated me! Congratulations!" << endl;
		}
	}
}
