/*
 * Boggle.cpp is the implementation of the Boggle class.
 * This is the explicit logic for maintaining
 * the output of the game.
 */

#include <cctype>
#include <stdexcept>
#include <string>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "lexicon.h"
#include "hashset.h"
#include "strlib.h"
#include "bogglegui.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

Boggle::Boggle(const Lexicon& dictionary, const string boardText) {
    localDictionary = dictionary;
    string boardValues = boardText;
    if(boardValues.empty()) boardValues = createRandomBoard();
    loadBoard(boardValues);
}

char Boggle::getLetter(const int row, const int col) {
    if(board.inBounds(row, col)) return board[row][col];
    throw invalid_argument("Cube does not exist.");
}

bool Boggle::checkWord(const string word) {
    if(word.length() < kMinWordLength) return false; 
    if(!localDictionary.contains(toLowerCase(word))) return false; 
    if(humanWords.contains(word)) return false;
    return true;
}

bool Boggle::humanWordSearch(const string word) {
    int strIndex = 0;
    BoggleGUI::clearHighlighting();
    for(int row = 0; row < kBoardRows; row++) {
        for(int col = 0; col < kBoardCols; col++) {
            HashSet< Vector<int> > checkedCubes;
            BoggleGUI::setAnimationDelay(kAnimationDelay);
            BoggleGUI::setHighlighted(row, col);
            if(board[row][col] == word[strIndex] && humanWordSearch(word, strIndex, checkedCubes, row, col)) {
                humanWords.add(word);
                BoggleGUI::setStatusMessage("You found a new word! \"" + word + "\"");
                BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                return true;
            }
            BoggleGUI::setHighlighted(row, col, false);
        }
    }
    BoggleGUI::clearHighlighting();
    BoggleGUI::setStatusMessage("That word can\'t be formed on this board.");
    return false;
}

bool Boggle::humanWordSearch(const string word, const int strIndex, 
                             HashSet< Vector<int> >& checkedCubes, const int row, const int col) {
    if(strIndex == word.length()) return true;
    for(int rowOffset = -1; rowOffset <= 1; rowOffset++) {
        for(int colOffset = -1; colOffset <= 1; colOffset++) {
            Vector<int> cube;
            int nextRow = row + rowOffset;
            int nextCol = col + colOffset;
            cube.add(nextRow);
            cube.add(nextCol);
            if(board.inBounds(nextRow, nextCol)) {
                BoggleGUI::setAnimationDelay(kAnimationDelay);
                BoggleGUI::setHighlighted(nextRow, nextCol);
                if(!checkedCubes.contains(cube) && board[nextRow][nextCol] == word[strIndex]) {
                    checkedCubes.add(cube);
                    if(humanWordSearch(word, strIndex+1, checkedCubes, nextRow, nextCol)) {
                        BoggleGUI::setHighlighted(nextRow, nextCol);
                        return true;
                    } else {
                        BoggleGUI::setHighlighted(nextRow, nextCol, false);
                        checkedCubes.remove(cube);
                    }
                    BoggleGUI::setHighlighted(nextRow, nextCol, false);   
                }
                BoggleGUI::setHighlighted(nextRow, nextCol, false);   
            }
        }
    }
    return false;
}

int Boggle::humanScore() {
    int score = 0;
    for(string word : humanWords) {
        score += word.length() / kMinWordLength + word.length() % kMinWordLength;
    } 
    BoggleGUI::setScore(score, BoggleGUI::HUMAN);
    return score;
}

Set<string> Boggle::computerWordSearch() {
    BoggleGUI::clearHighlighting();
    for(int row = 0; row < kBoardRows; row++) {
        for(int col = 0; col < kBoardCols; col++) {
            string prefix = "";
            HashSet< Vector<int> > checkedCubes;
            if(computerWordSearch("", checkedCubes, row, col));
        }
    }
    computerWords -= humanWords;
    return computerWords;
}

bool Boggle::computerWordSearch(const string prefix, HashSet< Vector<int> >& checkedCubes, const int row, const int col) {
    if(!localDictionary.containsPrefix(prefix)) return false;
    if(localDictionary.contains(prefix) && 
       prefix.length() >= kMinWordLength && !computerWords.contains(prefix)) {
        computerWords.add(prefix);
    }
    for(int rowOffset = -1; rowOffset <= 1; rowOffset++) {
        for(int colOffset = -1; colOffset <= 1; colOffset++) {
            Vector<int> nextCube;
            int nextRow = row + rowOffset;
            int nextCol = col + colOffset;
            nextCube.add(nextRow);
            nextCube.add(nextCol);
            if(board.inBounds(nextRow, nextCol) && !checkedCubes.contains(nextCube)) {
                checkedCubes.add(nextCube);
                char nextCh = board[nextRow][nextCol];
                if(computerWordSearch(prefix + nextCh, checkedCubes, nextRow, nextCol)) {
                    return true;
                } else {
                    checkedCubes.remove(nextCube);
                }
            }
        }
    }
    return false;
}

int Boggle::getScoreComputer() {
    int score = 0;
    for(string word : computerWords) {
        score += word.length() / kMinWordLength + word.length() % kMinWordLength;
    }
    BoggleGUI::setScore(score, BoggleGUI::COMPUTER);
    for(string word : computerWords) {
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
    }
    return score;
}

Set<string> Boggle::humanWordSet() {
    return humanWords;
}

string Boggle::createRandomBoard() {
    shuffle(CUBES, kBoardSize);
    string boardText;
    for(string cube : CUBES) {
        boardText += shuffle(cube);
    }
    return boardText;
}

void Boggle::loadBoard(const string boardText) {
    board.resize(kBoardRows, kBoardCols);
    for(int row = 0; row < kBoardRows; row++) {
        for(int col = 0; col < kBoardCols; col++) {
            int boardTextIndex = row * kBoardRows + col;
            board[row][col] = boardText[boardTextIndex];
        }
    }
    BoggleGUI::initialize(kBoardRows, kBoardCols);
    BoggleGUI::labelAllCubes(boardText);
    BoggleGUI::setStatusMessage("It's your turn!");
}

string Boggle::printBoard() {
    string boardGrid = "";
    for(int row = 0; row < kBoardRows; row++) {
        for(int col = 0; col < kBoardCols; col++) {
            boardGrid += board[row][col];
        }
        boardGrid += "\n";
    }
    return boardGrid;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    return out << boggle.printBoard();
}
