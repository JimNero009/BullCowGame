/* A class to control the BullCowGame, a simple 'guess the word' game.
All state information is stored in this class and all other parts of the code
need only interact with the game via the use of the specified public methods. */

#pragma once

#include <string>
#include <vector>

// Make the syntax Unreal friendly 
using FString = std::string;
using int32 = int;

struct FBullCowCount {
	// initialises bulls and cows to zero
	int32 Bulls = 0;
	int32 Cows = 0;
};

enum class EWordStatus { // class modifier means enum vals have smaller scope
	Invalid,
	OK,
	Not_Isogram,
	Wrong_Length,
	Not_Lower_Case
};

class FBullCowGame {
public:
	FBullCowGame();

	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	int32 GetHiddenWordLength() const;
	bool IsGameWon() const;

	EWordStatus CheckGuessValidity(FString) const;

	void Reset();
	FBullCowCount SubmitValidGuess(FString);

private:
	int32 MyCurrentTry;
	FString MyHiddenWord;
	bool bGameIsWon;
	bool bIsConstructed = false;

	bool IsIsogram(FString) const;
	bool IsLowerCase(FString) const;
	std::vector<FString> ReadFileOfIsogramsAndStore(FString FileName) const;
	FString ChooseRandomIsogram(std::vector<FString> IsogramVector) const;	
};