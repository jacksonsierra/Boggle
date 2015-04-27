/*
 * Boggle.h is the interface for playing
 * a game of Boggle. It includes helper
 * functions responsible for keeping track
 * of the game particulars, some of which
 * are public so to provide a client information
 * about the game
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"  //For dictionary
#include "set.h"  //For tracking words inputted
#include "grid.h"  //For Boggle board
#include "hashset.h"  //For tracking Boggle cubes visited
using namespace std;

enum Player { HUMAN, COMPUTER };  //For differentiating between game output messages

class Boggle {
public:
	/*
	 * Initializes a game of Boggle for which all
	 * helper functions for the Boggle class
	 * are accessible through.
	 * Parameters: 
	 * - Dicionary to be used in the game by
	 * by constant reference
	 * - the individual characters
	 * that display on the cubes that comprise the game as a string
	 */
    Boggle(const Lexicon& dictionary, const string boardText = "");

    /*
     * Returns the letter displayed on a specific cube in the
     * game, given its coordinates on the board
     * Parameters:
     * - coordinates on board as integers
     */
    char getLetter(const int row, const int col);
    
    /*
     * Determines whether a user-inputted word
     * is of requisite length, is in the aforementioned
     * dictionary, and has not been used previously
     * Paramteres:
     * - user-inputted word as string
     */
    bool checkWord(const string word);

    /*
     * Wrapper function for the recursive algorithm
     * that corroborates whether a user-inputted word
     * is contained in the Boggle game. It is responsible
     * for checking each cube in the game and then calling
     * a recursive function for further help, while also
     * updating the user interface to show the algorithm's
     * top-level progress.
     * Parameters:
     * - user-inputted word as string
     */
    bool humanWordSearch(const string word);

    /*
     * Returns the human Boggle player's score
     */
    int humanScore();

    /*
     * Wrapper function for the recursive algorithm that
     * finds all remaining words in the Boggle game
     * that the human user hasn't found. It returns its
     * findings as a Set of those words. It is responsible
     * for looping through each cube in the game and
     * then calling for help in determining words from the
     * recursive function.
     */
    Set<string> computerWordSearch();

    /*
     * Returns the computer Boggle player's score
     */
    int getScoreComputer();

    /*
     * Returns the set of words the human user
     * successfully enterered
     */
    Set<string> humanWordSet();

    /*
     * Returns a text version of the current
     * Boggle game suitable for console output
     */
    string printBoard();

    static const int kBoardSize = 16;  //Number of cubes in the Boggle game
    static const int kBoardRows = 4;  //Number of rows in the Boggle game
    static const int kBoardCols = 4;  //Number of columns in the Boggle game
    static const int kCubeSides = 6;  //Number of sides on the Boggle cube
    static const int kMinWordLength = 4;  //Minimum length for a Boggle word
    static const int kAnimationDelay = 80;  //Time in milliseconds between Boggle UI updates

private:
	/*
	 * Recursive function that determines whether a user-inputted word
	 * is contained in the Boggle game. This function receives a cube
	 * in the Boggle game and then evaluates that cube's neighboring
	 * cubes to see if a word can be formed from the one given.
	 * Parameters:
	 * - user-inputted word as string
	 * - index in the user-inputted word that is being checked
	 * - HashSet of Vectors of integers, which serve as the Board address, for reviewed cubes
	 * - cube coordinates in Boggle board as separate row/column integers
	 */
	bool humanWordSearch(const string word, const int strIndex, HashSet< Vector<int> >& checkedCubes, const int row, const int col);
    
	/*
	 * Recursive function that finds all valid Boggle words in the Boggle
	 * game and adds them to an instance variable that tracks this
	 * across helper functions. The function is responsible for looking at 
	 * all word possibilities that pass through a given Boggle cube. To
	 * speed up this process, it invokes branching pruning that terminates
	 * a potential word exploration if the first few characters in the word
	 * branch are not in the English language.
	 * Parameters:
	 * - prefix of current word branch being tested
	 * - HashSet of Vectors of integers, which serve as the Board address, for reviewed cubes
	 * - cube coordinates in Boggle board as separate row/column integers
	 */
    bool computerWordSearch(const string prefix, HashSet< Vector<int> >& checkedCubes, const int row, const int col);
    
    /*
     * Creates a random string of characters using the declared C-style
     * array of cube values at both the cube and character level -- in
     * other words, both the cube and characters of the cube are randomly
     * placed on the Boggle board
     */
    string createRandomBoard();

    /*
     * Reads in a string of characters and loads them into the
     * Boggle board by updating the Boggle board instance variable
     */
    void loadBoard(const string boardText);

	Grid<char> board;  //Boggle board data type
	Set<string> humanWords;  //Set of words the human user has inputted
	Set<string> computerWords;  //Set of words possible on the Boggle board less the human user's
	Lexicon localDictionary;  //Local copy of dictionary passed through game initialization
};

/*
 * Constructor for output stream of our Boggle data type to
 * a printable version
 */
ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
