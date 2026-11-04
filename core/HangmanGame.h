#pragma once
#include "WordBank.h"
#include <vector>  
#include "library/Result.h"
#include "HangmanResult.h"
#include "GameInitStatus.h"

class HangmanGame {
    private:
        WordBank bank;
        std::vector<char> guessedLetters;
        int wrongAttempts = 0;

        bool isLetterUsed(char guessLetter);
        void addUsedLetter(char letter);

        bool myIsAlpha(char ch);
        static char normalizeLetter(char letter);

    public:
        HangmanGame(const std::string& dictionaryPath);

        const Result<bool, GuessLetterError> guessLetter(const char letter);
        void openLetter(const char letter);
        Result<bool, CheckingLetterError> checkLetter(const char letter);
        GameInitStatus createWord();
        UserStatus isUserWin() const;
        GameInitStatus resetGame();

        const std::string& getEncryptedWord() const;
        const std::vector<char> getGuessedLetters() const;
        int getWrongAttempts() const;
        const std::string& getGuessedWord() const;

};