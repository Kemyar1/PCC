#include "../core/HangmanGame.h"
#include "TestData.h"
#include "vendor/catch.hpp"

#include <cstddef>
#include <string>


TEST_CASE("checkLetter_UnusedAsciiLetters_ReturnsSuccessWithoutChangingState", "[hangman]") {
    const bool expectedHasValue = true;
    const bool expectedValue = true;
    const bool expectedLettersAreEmpty = true;
    const int expectedWrongAttempts = 0;
    const std::string expectedMask = "*****";
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const std::string inputLetters = "azAZ";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);

    for (char letter : inputLetters) {
        auto result = game.checkLetter(letter);

        const bool actualHasValue = result.hasValue();
        REQUIRE(actualHasValue == expectedHasValue);

        const bool actualValue = result.value();
        REQUIRE(actualValue == expectedValue);
    }

    const auto guessedLetters = game.getGuessedLetters();
    const bool actualLettersAreEmpty = guessedLetters.empty();
    const int actualWrongAttempts = game.getWrongAttempts();
    const std::string actualMask = game.getEncryptedWord();
    REQUIRE(actualLettersAreEmpty == expectedLettersAreEmpty);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualMask == expectedMask);
}

TEST_CASE("checkLetter_NonAlphabeticCharacters_ReturnsInvalidType", "[hangman]") {
    const CheckingLetterError expectedError = CheckingLetterError::InvalidType;
    const bool expectedHasValue = false;
    const bool expectedLettersAreEmpty = true;
    const int expectedWrongAttempts = 0;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const char inputLetters[] = {'0', '.', ';', '\'', ' ', '\n', '\0'};
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);

    for (char letter : inputLetters) {
        auto result = game.checkLetter(letter);

        const bool actualHasValue = result.hasValue();
        REQUIRE(actualHasValue == expectedHasValue);

        const CheckingLetterError actualError = result.error();
        REQUIRE(actualError == expectedError);
    }

    const auto guessedLetters = game.getGuessedLetters();
    const bool actualLettersAreEmpty = guessedLetters.empty();
    const int actualWrongAttempts = game.getWrongAttempts();
    REQUIRE(actualLettersAreEmpty == expectedLettersAreEmpty);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
}

TEST_CASE("checkLetter_UsedLetter_ReturnsLetterWasUsed", "[hangman]") {
    const CheckingLetterError expectedError = CheckingLetterError::LetterWasUsed;
    const bool expectedHasValue = false;
    const bool expectedFirstGuessHasValue = true;
    const std::size_t expectedLetterCount = 1;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const char inputLetter = 'l';
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    auto firstGuess = game.guessLetter(inputLetter);
    const bool actualFirstGuessHasValue = firstGuess.hasValue();
    REQUIRE(actualFirstGuessHasValue == expectedFirstGuessHasValue);

    auto result = game.checkLetter(inputLetter);

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const CheckingLetterError actualError = result.error();
    const auto guessedLetters = game.getGuessedLetters();
    const std::size_t actualLetterCount = guessedLetters.size();
    REQUIRE(actualError == expectedError);
    REQUIRE(actualLetterCount == expectedLetterCount);
}

TEST_CASE("guessLetter_LetterInWord_RevealsAllOccurrencesWithoutMistake", "[hangman]") {
    const bool expectedHasValue = true;
    const bool expectedValue = true;
    const std::string expectedMask = "l***l";
    const int expectedWrongAttempts = 0;
    const std::size_t expectedLetterCount = 1;
    const char expectedUsedLetter = 'l';
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const char inputLetter = 'l';
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);

    auto result = game.guessLetter(inputLetter);

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const bool actualValue = result.value();
    const std::string actualMask = game.getEncryptedWord();
    const int actualWrongAttempts = game.getWrongAttempts();
    const auto guessedLetters = game.getGuessedLetters();
    const std::size_t actualLetterCount = guessedLetters.size();
    REQUIRE(actualValue == expectedValue);
    REQUIRE(actualMask == expectedMask);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualLetterCount == expectedLetterCount);

    const char actualUsedLetter = guessedLetters.front();
    REQUIRE(actualUsedLetter == expectedUsedLetter);
}

TEST_CASE("guessLetter_LetterOutsideWord_AddsOneMistakeAndRecordsLetter", "[hangman]") {
    const GuessLetterError expectedError = GuessLetterError::LetterNotFound;
    const bool expectedHasValue = false;
    const int expectedWrongAttempts = 1;
    const std::string expectedMask = "*****";
    const std::size_t expectedLetterCount = 1;
    const char expectedUsedLetter = 'a';
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const char inputLetter = 'a';
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);

    auto result = game.guessLetter(inputLetter);

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const GuessLetterError actualError = result.error();
    const int actualWrongAttempts = game.getWrongAttempts();
    const std::string actualMask = game.getEncryptedWord();
    const auto guessedLetters = game.getGuessedLetters();
    const std::size_t actualLetterCount = guessedLetters.size();
    REQUIRE(actualError == expectedError);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualMask == expectedMask);
    REQUIRE(actualLetterCount == expectedLetterCount);

    const char actualUsedLetter = guessedLetters.front();
    REQUIRE(actualUsedLetter == expectedUsedLetter);
}

TEST_CASE("guessLetter_RepeatedCorrectLetter_ReturnsLetterWasUsedWithoutChangingState", "[hangman]") {
    const GuessLetterError expectedError = GuessLetterError::LetterWasUsed;
    const bool expectedHasValue = false;
    const bool expectedFirstGuessHasValue = true;
    const std::string expectedMask = "l***l";
    const int expectedWrongAttempts = 0;
    const std::size_t expectedLetterCount = 1;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const char inputLetter = 'l';
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    auto firstGuess = game.guessLetter(inputLetter);
    const bool actualFirstGuessHasValue = firstGuess.hasValue();
    REQUIRE(actualFirstGuessHasValue == expectedFirstGuessHasValue);

    auto result = game.guessLetter(inputLetter);

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const GuessLetterError actualError = result.error();
    const std::string actualMask = game.getEncryptedWord();
    const int actualWrongAttempts = game.getWrongAttempts();
    const auto guessedLetters = game.getGuessedLetters();
    const std::size_t actualLetterCount = guessedLetters.size();
    REQUIRE(actualError == expectedError);
    REQUIRE(actualMask == expectedMask);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualLetterCount == expectedLetterCount);
}

TEST_CASE("guessLetter_RepeatedWrongLetter_DoesNotAddAnotherMistake", "[hangman]") {
    const GuessLetterError expectedError = GuessLetterError::LetterWasUsed;
    const bool expectedHasValue = false;
    const bool expectedFirstGuessHasValue = false;
    const int expectedWrongAttempts = 1;
    const std::size_t expectedLetterCount = 1;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const char inputLetter = 'a';
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    auto firstGuess = game.guessLetter(inputLetter);
    const bool actualFirstGuessHasValue = firstGuess.hasValue();
    REQUIRE(actualFirstGuessHasValue == expectedFirstGuessHasValue);

    auto result = game.guessLetter(inputLetter);

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const GuessLetterError actualError = result.error();
    const int actualWrongAttempts = game.getWrongAttempts();
    const auto guessedLetters = game.getGuessedLetters();
    const std::size_t actualLetterCount = guessedLetters.size();
    REQUIRE(actualError == expectedError);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualLetterCount == expectedLetterCount);
}

TEST_CASE("guessLetter_InvalidCharacter_DoesNotChangeGameState", "[hangman]") {
    const GuessLetterError expectedError = GuessLetterError::InvalidType;
    const bool expectedHasValue = false;
    const std::string expectedMask = "*****";
    const int expectedWrongAttempts = 0;
    const bool expectedLettersAreEmpty = true;
    const UserStatus expectedUserStatus = UserStatus::Playing;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const char inputLetters[] = {'1', '.', ' ', '\0'};
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);

    for (char letter : inputLetters) {
        auto result = game.guessLetter(letter);

        const bool actualHasValue = result.hasValue();
        REQUIRE(actualHasValue == expectedHasValue);

        const GuessLetterError actualError = result.error();
        REQUIRE(actualError == expectedError);
    }

    const std::string actualMask = game.getEncryptedWord();
    const int actualWrongAttempts = game.getWrongAttempts();
    const auto guessedLetters = game.getGuessedLetters();
    const bool actualLettersAreEmpty = guessedLetters.empty();
    const UserStatus actualUserStatus = game.isUserWin();
    REQUIRE(actualMask == expectedMask);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualLettersAreEmpty == expectedLettersAreEmpty);
    REQUIRE(actualUserStatus == expectedUserStatus);
}

TEST_CASE("isUserWin_NewRound_ReturnsPlaying", "[hangman]") {
    const UserStatus expectedUserStatus = UserStatus::Playing;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);

    const UserStatus actualUserStatus = game.isUserWin();

    REQUIRE(actualUserStatus == expectedUserStatus);
}

TEST_CASE("isUserWin_AllLettersGuessed_ReturnsWon", "[hangman]") {
    const UserStatus expectedIntermediateStatus = UserStatus::Playing;
    const UserStatus expectedFinalStatus = UserStatus::Won;
    const std::string expectedMask = "level";
    const bool expectedGuessHasValue = true;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const char firstLetter = 'l';
    const char secondLetter = 'e';
    const char thirdLetter = 'v';
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);

    auto firstGuess = game.guessLetter(firstLetter);
    const bool actualFirstGuessHasValue = firstGuess.hasValue();
    REQUIRE(actualFirstGuessHasValue == expectedGuessHasValue);
    const UserStatus actualFirstStatus = game.isUserWin();
    REQUIRE(actualFirstStatus == expectedIntermediateStatus);

    auto secondGuess = game.guessLetter(secondLetter);
    const bool actualSecondGuessHasValue = secondGuess.hasValue();
    REQUIRE(actualSecondGuessHasValue == expectedGuessHasValue);
    const UserStatus actualSecondStatus = game.isUserWin();
    REQUIRE(actualSecondStatus == expectedIntermediateStatus);

    auto thirdGuess = game.guessLetter(thirdLetter);
    const bool actualThirdGuessHasValue = thirdGuess.hasValue();
    REQUIRE(actualThirdGuessHasValue == expectedGuessHasValue);

    const UserStatus actualFinalStatus = game.isUserWin();
    const std::string actualMask = game.getEncryptedWord();
    REQUIRE(actualFinalStatus == expectedFinalStatus);
    REQUIRE(actualMask == expectedMask);
}

TEST_CASE("isUserWin_FiveWrongGuesses_ReturnsPlaying", "[hangman]") {
    const UserStatus expectedUserStatus = UserStatus::Playing;
    const int expectedWrongAttempts = 5;
    const bool expectedHasValue = false;
    const GuessLetterError expectedError = GuessLetterError::LetterNotFound;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const std::string inputLetters = "abcdf";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);

    for (char letter : inputLetters) {
        auto result = game.guessLetter(letter);

        const bool actualHasValue = result.hasValue();
        REQUIRE(actualHasValue == expectedHasValue);

        const GuessLetterError actualError = result.error();
        REQUIRE(actualError == expectedError);
    }

    const int actualWrongAttempts = game.getWrongAttempts();
    const UserStatus actualUserStatus = game.isUserWin();
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualUserStatus == expectedUserStatus);
}

TEST_CASE("isUserWin_SixWrongGuesses_ReturnsLost", "[hangman]") {
    const UserStatus expectedUserStatus = UserStatus::Lost;
    const int expectedWrongAttempts = 6;
    const std::string expectedMask = "*****";
    const bool expectedHasValue = false;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const std::string inputLetters = "abcdfg";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);

    for (char letter : inputLetters) {
        auto result = game.guessLetter(letter);
        const bool actualHasValue = result.hasValue();
        REQUIRE(actualHasValue == expectedHasValue);
    }

    const int actualWrongAttempts = game.getWrongAttempts();
    const UserStatus actualUserStatus = game.isUserWin();
    const std::string actualMask = game.getEncryptedWord();
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualUserStatus == expectedUserStatus);
    REQUIRE(actualMask == expectedMask);
}

TEST_CASE("resetGame_PlayedRound_ClearsMistakesLettersAndMask", "[hangman]") {
    const int expectedWrongAttempts = 0;
    const bool expectedLettersAreEmpty = true;
    const std::string expectedMask = "*****";
    const UserStatus expectedUserStatus = UserStatus::Playing;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const int expectedWrongAttemptsBeforeReset = 1;
    const bool expectedCorrectGuessHasValue = true;
    const bool expectedWrongGuessHasValue = false;
    const char correctLetter = 'l';
    const char wrongLetter = 'a';
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    auto correctGuess = game.guessLetter(correctLetter);
    const bool actualCorrectGuessHasValue = correctGuess.hasValue();
    REQUIRE(actualCorrectGuessHasValue == expectedCorrectGuessHasValue);
    auto wrongGuess = game.guessLetter(wrongLetter);
    const bool actualWrongGuessHasValue = wrongGuess.hasValue();
    REQUIRE(actualWrongGuessHasValue == expectedWrongGuessHasValue);
    const int actualWrongAttemptsBeforeReset = game.getWrongAttempts();
    REQUIRE(actualWrongAttemptsBeforeReset == expectedWrongAttemptsBeforeReset);

    const GameInitStatus actualResetStatus = game.resetGame();
    REQUIRE(actualResetStatus == expectedInitStatus);

    const int actualWrongAttempts = game.getWrongAttempts();
    const auto guessedLetters = game.getGuessedLetters();
    const bool actualLettersAreEmpty = guessedLetters.empty();
    const std::string actualMask = game.getEncryptedWord();
    const UserStatus actualUserStatus = game.isUserWin();
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualLettersAreEmpty == expectedLettersAreEmpty);
    REQUIRE(actualMask == expectedMask);
    REQUIRE(actualUserStatus == expectedUserStatus);

    auto nextGuess = game.guessLetter(correctLetter);
    const bool actualNextGuessHasValue = nextGuess.hasValue();
    REQUIRE(actualNextGuessHasValue == expectedCorrectGuessHasValue);
}

TEST_CASE("resetGame_FinishedRound_StartsPlaying", "[hangman]") {
    const UserStatus expectedUserStatus = UserStatus::Playing;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const int expectedWrongAttempts = 0;
    const bool expectedLettersAreEmpty = true;
    const std::string expectedMask = "*****";
    const bool expectedCorrectGuessHasValue = true;
    const bool expectedWrongGuessHasValue = false;
    const UserStatus expectedVictoryStatus = UserStatus::Won;
    const UserStatus expectedDefeatStatus = UserStatus::Lost;
    const std::string correctLetters = "lev";
    const std::string wrongLetters = "abcdfg";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    SECTION("After victory") {
        for (char letter : correctLetters) {
            auto result = game.guessLetter(letter);
            const bool actualHasValue = result.hasValue();
            REQUIRE(actualHasValue == expectedCorrectGuessHasValue);
        }
        const UserStatus actualVictoryStatus = game.isUserWin();
        REQUIRE(actualVictoryStatus == expectedVictoryStatus);
    }
    SECTION("After defeat") {
        for (char letter : wrongLetters) {
            auto result = game.guessLetter(letter);
            const bool actualHasValue = result.hasValue();
            REQUIRE(actualHasValue == expectedWrongGuessHasValue);
        }
        const UserStatus actualDefeatStatus = game.isUserWin();
        REQUIRE(actualDefeatStatus == expectedDefeatStatus);
    }

    const GameInitStatus actualResetStatus = game.resetGame();
    REQUIRE(actualResetStatus == expectedInitStatus);

    const UserStatus actualUserStatus = game.isUserWin();
    const int actualWrongAttempts = game.getWrongAttempts();
    const auto guessedLetters = game.getGuessedLetters();
    const bool actualLettersAreEmpty = guessedLetters.empty();
    const std::string actualMask = game.getEncryptedWord();
    REQUIRE(actualUserStatus == expectedUserStatus);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualLettersAreEmpty == expectedLettersAreEmpty);
    REQUIRE(actualMask == expectedMask);
}

TEST_CASE("resetGame_EmptyDictionary_ReturnsIndexWordNotExist", "[hangman]") {
    const GameInitStatus expectedStatus = GameInitStatus::IndexWordNotExist;
    const std::string filename = "empty.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);

    const GameInitStatus actualStatus = game.resetGame();

    REQUIRE(actualStatus == expectedStatus);
}

TEST_CASE("guessLetter_UppercaseLetter_RevealsLowercaseWordWithoutMistake", "[hangman][regression]") {
    const bool expectedHasValue = true;
    const std::string expectedMask = "l***l";
    const int expectedWrongAttempts = 0;
    const std::size_t expectedLetterCount = 1;
    const char expectedUsedLetter = 'l';
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const char inputLetter = 'L';
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);

    auto result = game.guessLetter(inputLetter);

    const bool actualHasValue = result.hasValue();
    const std::string actualMask = game.getEncryptedWord();
    const int actualWrongAttempts = game.getWrongAttempts();
    const auto guessedLetters = game.getGuessedLetters();
    const std::size_t actualLetterCount = guessedLetters.size();
    REQUIRE(actualHasValue == expectedHasValue);
    REQUIRE(actualMask == expectedMask);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualLetterCount == expectedLetterCount);

    const char actualUsedLetter = guessedLetters.front();
    REQUIRE(actualUsedLetter == expectedUsedLetter);
}

TEST_CASE("checkLetter_SameLetterInDifferentCase_ReturnsLetterWasUsed", "[hangman][regression]") {
    const CheckingLetterError expectedError = CheckingLetterError::LetterWasUsed;
    const bool expectedHasValue = false;
    const bool expectedFirstGuessHasValue = true;
    const int expectedWrongAttempts = 0;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const char firstLetter = 'l';
    const char inputLetter = 'L';
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    auto firstGuess = game.guessLetter(firstLetter);
    const bool actualFirstGuessHasValue = firstGuess.hasValue();
    REQUIRE(actualFirstGuessHasValue == expectedFirstGuessHasValue);

    auto result = game.checkLetter(inputLetter);

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const CheckingLetterError actualError = result.error();
    const int actualWrongAttempts = game.getWrongAttempts();
    REQUIRE(actualError == expectedError);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
}

TEST_CASE("isUserWin_EmptyDictionary_ReturnsPlayingWithoutFalseWin", "[hangman][regression]") {
    const UserStatus expectedUserStatus = UserStatus::Playing;
    const GameInitStatus expectedInitStatus = GameInitStatus::IndexWordNotExist;
    const std::string filename = "empty.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);

    const UserStatus actualUserStatus = game.isUserWin();

    REQUIRE(actualUserStatus == expectedUserStatus);
}
