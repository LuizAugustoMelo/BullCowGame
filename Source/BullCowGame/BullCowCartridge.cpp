// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordsList.h"
#include "Kismet/KismetSystemLibrary.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
	Super::BeginPlay();

	SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
	if (bGameOver)
	{
		if (PlayerInput.ToLower() == "y")
			SetupGame();
		else if (PlayerInput.ToLower() == "n")
			GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	}
	else
	{
		ProcessGuess(PlayerInput);
	}
}

void UBullCowCartridge::SetupGame()
{
	FString Word = "";
	do
	{
		Word = Words4Char[FMath::RandRange(0, (Words4Char.Num() - 1))];

	} while (!IsIsogram(Word));

	HiddenWord = Word;

	if (Lives < (HiddenWord.Len() * 2))
		Lives = HiddenWord.Len() * 2;

	NumberTry = Lives;
	bGameOver = false;

	ClearScreen();
	PrintLine(TEXT("Welcome to Bull & Cows"));

	/// <summary>
	/// This is a Debbug Message
	/// </summary>
	PrintLine(TEXT("The word sorted is: %s"), *HiddenWord);

	PrintLine(TEXT("The hidden word is %i letter word!"), HiddenWord.Len());
	PrintLine(TEXT("Type in your guess.\npress ENTER to continue..."));
}

void UBullCowCartridge::EndGame()
{
	bGameOver = true;
	PrintLine(TEXT("\nDo you want to play again? (y/n)"));
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
	for (int32 Index = 0; Index < Word.Len(); Index++)
	{
		for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
		{
			if (Word[Index] == Word[Comparison])
				return false;
		}
	}
	return true;
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
	if (Guess == HiddenWord)
	{
		PrintLine(TEXT("You have Won!"));
		EndGame();
		return;
	}

	//Check Right Number of Characters
	if (Guess.Len() != HiddenWord.Len())
	{
		PrintLine(TEXT("Sorry, try guessing again!\nYou have %i attempts remaining."), NumberTry);
		PrintLine(TEXT("The hidden word is %i letter word!"), HiddenWord.Len());
		return;
	}

	//Check if Isogram
	if (!IsIsogram(Guess))
	{
		/* Code */
		PrintLine(TEXT("No repeating letters, guess again."));
		return;
	}

	//Remove Attempt
	PrintLine(TEXT("Lost Attempt!"));
	--NumberTry;

	//Check if Attempt > 0
	if (NumberTry <= 0)
	{
		//If No Show gameover
		ClearScreen();
		PrintLine(TEXT("You have no attempts left!"));
		PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
		PrintLine(TEXT("\nGameOver!"));
		EndGame();
		return;
	}

	//Show the player Bulls and Cows
	FBullCowCount Score = GetBollCows(Guess);

	PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

	PrintLine(TEXT("Guess again, you have %i attempts left."), NumberTry);
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
	TArray<FString> ValidWords;

	for (FString Word : WordList)
	{
		if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
		{
			ValidWords.Emplace(Word);
		}
	}

	return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBollCows(const FString& Guess) const
{
	FBullCowCount Count;

	for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
	{
		if (Guess[GuessIndex] == HiddenWord[GuessIndex])
		{
			Count.Bulls++;
			continue;
		}

		for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
		{
			if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
			{
				Count.Cows++;
				break;
			}
		}
	}

	return Count;
}