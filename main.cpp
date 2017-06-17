/* This is the console executable that makes use of the BullCow Class
This acts as the view in a MVC patttern, and is responsible for all user interaction.
For game logic see the FBullCowGame class.
*/

#pragma once

#include <string>
#include <iostream>
#include "FBullCowGame.h"

// Make the syntax Unreal friendly
using FText = std::string;
using int32 = int;

void PrintIntro();
FText GetValidGuess();
void PlayGame();
bool AskToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGame; // Instantiate the game, which we reset if a new game is requested 

int main() {
	// TODO allow for game to be played again with same word, diferent word, or quit game
	bool bPlayAgain = false;
	do {
		PrintIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	} while (bPlayAgain);
	return 0;
}

void PrintIntro() {
	/* Print out ASCII art and information for how to play */
	std::cout << "\n\nWelcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "          }   {         ___ " << std::endl;
	std::cout << "          (o o)        (o o) " << std::endl;
	std::cout << "   /-------\\ /          \\ /-------\\ " << std::endl;
	std::cout << "  / | BULL |O            O| COW  | \\ " << std::endl;
	std::cout << " *  |-,--- |              |------|  * " << std::endl;
	std::cout << "    ^      ^              ^      ^ " << std::endl;
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength()
	          << "-letter isogram I am thinking of?\n";
	std::cout << std::endl;
}

FText GetValidGuess() {
	/* Loop around until the user enters a valid guess, to be passed to the game */
	EWordStatus Status = EWordStatus::Invalid;
	FText Guess = "";
	//std::getline(std::cin, Guess);
	do {
		std::cout << "Try " << BCGame.GetCurrentTry() << " out of " << BCGame.GetMaxTries() << ". Please input your guess: ";
		std::getline(std::cin, Guess);
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status) {
		case EWordStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << "-letter word.\n\n";
			break;
		case EWordStatus::Not_Isogram:
			std::cout << "Please enter an isogram (no repeated letters)\n\n";
			break;
		case EWordStatus::Not_Lower_Case:
			std::cout << "Please enter a word that is all in lower case.\n\n";
			break;
		default:
			// assume is valid
			break;			
		}
	} while (Status != EWordStatus::OK); // loop until no errors
	return Guess;
}

void PlayGame() {
	/* Plays a single game until either the player wins or they have taken all their tries */
	BCGame.Reset();
	const int32 MaxTries = BCGame.GetMaxTries();
	// while game is NOT won and still tries remaining, ask for guesses
	while(!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries) {
		FText Guess = GetValidGuess();
		
		// Submit VALID guess to the game if valid
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);
		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << "\n\n";
	}

	PrintGameSummary();
	return;
}

bool AskToPlayAgain() {
	std::cout << "Do you want to play again with the same hidden word? (y/n)" << std::endl;
	FText Response = "";
	std::getline(std::cin, Response);
	return (Response[0] == 'Y') || (Response[0] == 'y');
}

void PrintGameSummary() {
	/* Return information about how the game ended */
	if (BCGame.IsGameWon()) {
		std::cout << "Well done, you won the game!\n";
	} else {
		std::cout << "Oh well. Better luck next time!\n";
	}
	return;
}
