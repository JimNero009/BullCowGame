#pragma once

#include "FBullCowGame.h"
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>

#define TMap std::map

// Make the synatx Unreal friendly
using FString = std::string;
using int32 = int;

FBullCowGame::FBullCowGame(){ FBullCowGame::Reset(); }

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const {
	/* Depending on the word length, return the number of max tries the user should have */
	TMap<int32, int32> WordLengthToMaxTries{ {3,4}, {4,7}, {5,10}, {6,15}, {7,20} };
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

EWordStatus FBullCowGame::CheckGuessValidity(FString Guess) const {
	if (!IsIsogram(Guess)) {// if guess isn't an isogram, return error
		return EWordStatus::Not_Isogram; 
	} else if (!IsLowerCase(Guess)) { // if guess isn't all lower case, return error. This also checks if non-alphabet chars are included.
		return EWordStatus::Not_Lower_Case;
	} else if (GetHiddenWordLength() != Guess.length()) { // if guess length if wrong ,return error
		return EWordStatus::Wrong_Length;
	} else {
		// otherwise, return ok
		return EWordStatus::OK;
	}
}

void FBullCowGame::Reset(){
	/* Resets game parameters so that the game can be played again from scratch */
	if (!bIsConstructed) { // TODO put this in a seperate function
		std::cout << "We will begin by setting the length of the isogram to guess.\n\n";
		int WordLength = 0;
		while (WordLength < 3 || WordLength > 7) {
			std::cout << "Please enter a number between 3 and 7, inclusive. \n";
			std::cin >> WordLength;
		}
		const FString FILE_NAME = "Data/Isogram_" + std::to_string(WordLength);
		std::vector<FString> Isograms = ReadFileOfIsogramsAndStore(FILE_NAME);
		const FString HIDDEN_WORD = ChooseRandomIsogram(Isograms);
		MyHiddenWord = HIDDEN_WORD;
		bIsConstructed = true;
	}
	bGameIsWon = false;
	MyCurrentTry = 1;	

	return;
}


FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess){
	/* Takes in a valid guess and returns the BullCowCount for that guess */
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	// loop through each letter in hidden word - assume same length as guess
	int32 WordLength = MyHiddenWord.length();
	for (int32 i = 0; i < WordLength; i++) {
		// compare letters against hidden word
		for (int32 j = 0; j < WordLength; j++) {
			// if match, increment bulls if in same place, cows if not
			if (Guess[j] == MyHiddenWord[i]) {
				if (i == j) {
					BullCowCount.Bulls++;
				} else {
					BullCowCount.Cows++;
				}
			} 
		}
	}
	if (BullCowCount.Bulls == WordLength) {
		bGameIsWon = true;
	} else {
		bGameIsWon = false;
	}
	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Guess) const {
	// Treat 0 and 1 letter string as isograms
	if (Guess.length() < 2) { return true; }

	// implement map
	TMap<char, bool> LettersSeen;
	for (auto Letter : Guess) {
		Letter = tolower(Letter); // to handle both upper and lower case
		if (LettersSeen[Letter]) { // if the letter is in there
			return false; // not an isogram
		} else { // otherwise, put it in there
			LettersSeen[Letter] = true;
		}
	}

	return true;
}

bool FBullCowGame::IsLowerCase(FString Guess) const {
	for (auto Letter : Guess) { // loop through and test if each character is lowercase
		if (! islower(Letter)) {
			return false;
		}
	}
	return true;
}

std::vector<FString> FBullCowGame::ReadFileOfIsogramsAndStore(FString FileName) const {
	/* Get isogram data from a file */
	std::fstream IsogramFile;
	std::vector<FString> Isograms;
	IsogramFile.open(FileName + ".txt", std::fstream::in);
	if (IsogramFile.is_open()) {		
		std::string IsogramInFile = "";
		while (std::getline(IsogramFile, IsogramInFile)) {
			Isograms.push_back(IsogramInFile);
		}
		IsogramFile.close();
		std::getline(std::cin, IsogramInFile); // Removes garbage that hangs around after this TODO figure out why this happens
	}
	
	return Isograms;
}

FString FBullCowGame::ChooseRandomIsogram(std::vector<FString> IsogramVector) const {
	/* Given a vector of isograms, pick one at random */
	srand(time(NULL)); 
	const int RandomChoice = rand() % IsogramVector.size();
	return IsogramVector[RandomChoice];
}
