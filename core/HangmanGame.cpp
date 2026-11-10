// Core game rules: validate guesses, reveal letters and determine the round outcome.
#include "HangmanGame.h"

using CheckResultLetter = Result<bool, CheckingLetterError>;

HangmanGame::HangmanGame(const std::string& dictionaryPath)
    : bank(dictionaryPath) {}


bool HangmanGame::myIsAlpha(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

// Compare and store guesses in lowercase so letter case does not affect repeats.
char HangmanGame::normalizeLetter(char letter) {
    if (letter >= 'A' && letter <= 'Z') {
        return static_cast<char>(letter - 'A' + 'a');
    }

    return letter;
}

bool HangmanGame::isLetterUsed(char guessLetter) {
    for (char letter : guessedLetters) {
        if (letter == guessLetter) return true;
    }

    return false; 
}

void HangmanGame::addUsedLetter(const char letter) {
    guessedLetters.push_back(letter);
}

// Validate first: invalid or repeated guesses must leave the round state unchanged.
const Result<bool, GuessLetterError> HangmanGame::guessLetter(const char letter) {
    const char normalizedLetter = normalizeLetter(letter);
    auto checkingLetter = checkLetter(normalizedLetter);
    if (!checkingLetter.hasValue()) {
        switch (checkingLetter.error()) {
            case CheckingLetterError::LetterWasUsed:
                return Result<bool, GuessLetterError>::failure(GuessLetterError::LetterWasUsed);

            case CheckingLetterError::InvalidType:
                return Result<bool, GuessLetterError>::failure(GuessLetterError::InvalidType);
        }
    }

    bool isLetterFound = false;
    addUsedLetter(normalizedLetter);
    for (char letterInWord : bank.getGuessedWord()) {
        if (normalizedLetter == letterInWord) {
            openLetter(normalizedLetter);
            isLetterFound = true;
        }
    }

    // Only a new valid letter absent from the word costs an attempt.
    if (!isLetterFound) {
        wrongAttempts++;
        return Result<bool, GuessLetterError>::failure(GuessLetterError::LetterNotFound);
    }

    return Result<bool, GuessLetterError>::success(true);
}

void HangmanGame::openLetter(const char letter) {
    bank.openLetter(letter);
}

CheckResultLetter HangmanGame::checkLetter(const char letter) {
    const char normalizedLetter = normalizeLetter(letter);
    if (!myIsAlpha(normalizedLetter)) {
        return CheckResultLetter::failure(CheckingLetterError::InvalidType);
    } 
    if (isLetterUsed(normalizedLetter)) {
        return CheckResultLetter::failure(CheckingLetterError::LetterWasUsed);
    }
    return CheckResultLetter::success(true);
}

GameInitStatus HangmanGame::createWord() {
    return bank.createWord();
}

UserStatus HangmanGame::isUserWin() const {
    if (wrongAttempts >= 6) {
        return UserStatus::Lost;
    }

    // A win requires a nonempty word with every letter revealed.
    if (!bank.getGuessedWord().empty()
        && bank.getEncryptedWord() == bank.getGuessedWord()
        && wrongAttempts < 6) {
        return UserStatus::Won;
    }

    return UserStatus::Playing;
}

GameInitStatus HangmanGame::resetGame() {
    wrongAttempts = 0;         
    guessedLetters.clear();

    return createWord();
}

const std::vector<char> HangmanGame::getGuessedLetters() const {
    return guessedLetters;
}

int HangmanGame::getWrongAttempts() const {
    return wrongAttempts;
}

const std::string& HangmanGame::getEncryptedWord() const {
    return bank.getEncryptedWord();
}

const std::string& HangmanGame::getGuessedWord() const {
    return bank.getGuessedWord();
}
