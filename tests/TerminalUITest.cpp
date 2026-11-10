// String streams supply test input and capture UI output without using the terminal.
#include "../ui/TerminalUI.h"
#include "TestData.h"
#include "vendor/catch.hpp"

#include <sstream>
#include <string>

TEST_CASE("handleTypeLetter_CorrectLetter_OpensLetterAndPrintsSuccess", "[ui]") {
    const std::string expectedMask = "l***l";
    const std::string expectedMessage = "Good guess";
    const bool expectedHasValue = true;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const auto notFoundPosition = std::string::npos;
    const std::string inputText = "l\n";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    auto result = ui.handleTypeLetter();

    const bool actualHasValue = result.hasValue();
    const std::string actualMask = game.getEncryptedWord();
    const std::string actualOutput = output.str();
    const auto actualMessagePosition = actualOutput.find(expectedMessage);
    REQUIRE(actualHasValue == expectedHasValue);
    REQUIRE(actualMask == expectedMask);
    REQUIRE(actualMessagePosition != notFoundPosition);
}

TEST_CASE("handleTypeLetter_WrongLetter_AddsMistakeAndPrintsFeedback", "[ui]") {
    const GuessLetterError expectedError = GuessLetterError::LetterNotFound;
    const int expectedWrongAttempts = 1;
    const std::string expectedMessage = "No new letter";
    const bool expectedHasValue = false;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const auto notFoundPosition = std::string::npos;
    const std::string inputText = "a\n";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    auto result = ui.handleTypeLetter();

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const GuessLetterError actualError = result.error();
    const int actualWrongAttempts = game.getWrongAttempts();
    const std::string actualOutput = output.str();
    const auto actualMessagePosition = actualOutput.find(expectedMessage);
    REQUIRE(actualError == expectedError);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualMessagePosition != notFoundPosition);
}

TEST_CASE("handleTypeLetter_RepeatedLetter_PrintsDuplicateErrorWithoutNewMistake", "[ui]") {
    const GuessLetterError expectedError = GuessLetterError::LetterWasUsed;
    const int expectedWrongAttempts = 1;
    const std::string expectedMessage = "already used";
    const bool expectedHasValue = false;
    const bool expectedFirstGuessHasValue = false;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const auto notFoundPosition = std::string::npos;
    const char firstLetter = 'a';
    const std::string inputText = "a\n";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    auto firstGuess = game.guessLetter(firstLetter);
    const bool actualFirstGuessHasValue = firstGuess.hasValue();
    REQUIRE(actualFirstGuessHasValue == expectedFirstGuessHasValue);
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    auto result = ui.handleTypeLetter();

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const GuessLetterError actualError = result.error();
    const int actualWrongAttempts = game.getWrongAttempts();
    const std::string actualOutput = output.str();
    const auto actualMessagePosition = actualOutput.find(expectedMessage);
    REQUIRE(actualError == expectedError);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualMessagePosition != notFoundPosition);
}

TEST_CASE("handleTypeLetter_MultipleCharacters_ReturnsInvalidTypeWithoutChangingState", "[ui]") {
    const GuessLetterError expectedError = GuessLetterError::InvalidType;
    const bool expectedHasValue = false;
    const int expectedWrongAttempts = 0;
    const bool expectedLettersAreEmpty = true;
    const std::string expectedMask = "*****";
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const std::string numericInput = "123\n";
    const std::string severalLettersInput = "lev\n";
    std::string inputText;
    SECTION("Numbers") { inputText = numericInput; }
    SECTION("Several letters") { inputText = severalLettersInput; }
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    auto result = ui.handleTypeLetter();

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const GuessLetterError actualError = result.error();
    const int actualWrongAttempts = game.getWrongAttempts();
    const auto guessedLetters = game.getGuessedLetters();
    const bool actualLettersAreEmpty = guessedLetters.empty();
    const std::string actualMask = game.getEncryptedWord();
    REQUIRE(actualError == expectedError);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualLettersAreEmpty == expectedLettersAreEmpty);
    REQUIRE(actualMask == expectedMask);
}

TEST_CASE("handleTypeLetter_Punctuation_ReturnsInvalidTypeWithoutChangingState", "[ui]") {
    const GuessLetterError expectedError = GuessLetterError::InvalidType;
    const bool expectedHasValue = false;
    const int expectedWrongAttempts = 0;
    const bool expectedLettersAreEmpty = true;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const std::string inputText = ".\n";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    auto result = ui.handleTypeLetter();

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const GuessLetterError actualError = result.error();
    const int actualWrongAttempts = game.getWrongAttempts();
    const auto guessedLetters = game.getGuessedLetters();
    const bool actualLettersAreEmpty = guessedLetters.empty();
    REQUIRE(actualError == expectedError);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualLettersAreEmpty == expectedLettersAreEmpty);
}

// Intended input rule for these tests: one complete line = one guess.
TEST_CASE("handleTypeLetter_LineWithTwoLetters_ReturnsInvalidTypeWithoutChangingState", "[ui][regression]") {
    const GuessLetterError expectedError = GuessLetterError::InvalidType;
    const bool expectedHasValue = false;
    const int expectedWrongAttempts = 0;
    const bool expectedLettersAreEmpty = true;
    const std::string expectedMask = "*****";
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const std::string inputText = "l e\n";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    auto result = ui.handleTypeLetter();

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const GuessLetterError actualError = result.error();
    const int actualWrongAttempts = game.getWrongAttempts();
    const auto guessedLetters = game.getGuessedLetters();
    const bool actualLettersAreEmpty = guessedLetters.empty();
    const std::string actualMask = game.getEncryptedWord();
    REQUIRE(actualError == expectedError);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);
    REQUIRE(actualLettersAreEmpty == expectedLettersAreEmpty);
    REQUIRE(actualMask == expectedMask);
}

TEST_CASE("handleTypeLetter_EmptyLine_ReturnsInvalidTypeWithoutConsumingNextLine", "[ui][regression]") {
    const GuessLetterError expectedError = GuessLetterError::InvalidType;
    const bool expectedHasValue = false;
    const bool expectedLettersAreEmpty = true;
    const int expectedWrongAttempts = 0;
    const bool expectedRemainingLineRead = true;
    const std::string expectedRemainingLine = "l";
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const std::string inputText = "\nl\n";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    auto result = ui.handleTypeLetter();

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const GuessLetterError actualError = result.error();
    const auto guessedLetters = game.getGuessedLetters();
    const bool actualLettersAreEmpty = guessedLetters.empty();
    const int actualWrongAttempts = game.getWrongAttempts();
    REQUIRE(actualError == expectedError);
    REQUIRE(actualLettersAreEmpty == expectedLettersAreEmpty);
    REQUIRE(actualWrongAttempts == expectedWrongAttempts);

    std::string actualRemainingLine;
    const bool actualRemainingLineRead = static_cast<bool>(
        std::getline(input, actualRemainingLine)
    );
    REQUIRE(actualRemainingLineRead == expectedRemainingLineRead);
    REQUIRE(actualRemainingLine == expectedRemainingLine);
}

TEST_CASE("handlePrintUserStatus_Playing_ReturnsPlayingWithoutFinalMessage", "[ui]") {
    const UserStatus expectedUserStatus = UserStatus::Playing;
    const bool expectedOutputIsEmpty = true;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const std::string inputText = "";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    const UserStatus actualUserStatus = ui.handlePrintUserStatus();

    const std::string actualOutput = output.str();
    const bool actualOutputIsEmpty = actualOutput.empty();
    REQUIRE(actualUserStatus == expectedUserStatus);
    REQUIRE(actualOutputIsEmpty == expectedOutputIsEmpty);
}

TEST_CASE("handlePrintUserStatus_Won_PrintsWinAndSecretWord", "[ui]") {
    const UserStatus expectedUserStatus = UserStatus::Won;
    const std::string expectedMessage = "YOU WIN";
    const std::string expectedWord = "level";
    const bool expectedGuessHasValue = true;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const auto notFoundPosition = std::string::npos;
    const std::string inputLetters = "lev";
    const std::string inputText = "";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    for (char letter : inputLetters) {
        auto result = game.guessLetter(letter);
        const bool actualHasValue = result.hasValue();
        REQUIRE(actualHasValue == expectedGuessHasValue);
    }
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    const UserStatus actualUserStatus = ui.handlePrintUserStatus();

    const std::string actualOutput = output.str();
    const auto actualMessagePosition = actualOutput.find(expectedMessage);
    const auto actualWordPosition = actualOutput.find(expectedWord);
    REQUIRE(actualUserStatus == expectedUserStatus);
    REQUIRE(actualMessagePosition != notFoundPosition);
    REQUIRE(actualWordPosition != notFoundPosition);
}

TEST_CASE("handlePrintUserStatus_Lost_PrintsGameOverAndSecretWord", "[ui]") {
    const UserStatus expectedUserStatus = UserStatus::Lost;
    const std::string expectedMessage = "GAME OVER";
    const std::string expectedWord = "level";
    const bool expectedGuessHasValue = false;
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const auto notFoundPosition = std::string::npos;
    const std::string inputLetters = "abcdfg";
    const std::string inputText = "";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    const GameInitStatus actualInitStatus = game.resetGame();
    REQUIRE(actualInitStatus == expectedInitStatus);
    for (char letter : inputLetters) {
        auto result = game.guessLetter(letter);
        const bool actualHasValue = result.hasValue();
        REQUIRE(actualHasValue == expectedGuessHasValue);
    }
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    const UserStatus actualUserStatus = ui.handlePrintUserStatus();

    const std::string actualOutput = output.str();
    const auto actualMessagePosition = actualOutput.find(expectedMessage);
    const auto actualWordPosition = actualOutput.find(expectedWord);
    REQUIRE(actualUserStatus == expectedUserStatus);
    REQUIRE(actualMessagePosition != notFoundPosition);
    REQUIRE(actualWordPosition != notFoundPosition);
}

TEST_CASE("execute_MissingDictionary_PrintsErrorWithoutReportingWin", "[ui][regression]") {
    const std::string forbiddenMessage = "YOU WIN";
    const auto expectedWinPosition = std::string::npos;
    const bool expectedOutputIsEmpty = false;
    const std::string inputText = "";
    const std::string filename = "does_not_exist.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    ui.execute();

    const std::string actualOutput = output.str();
    const auto actualWinPosition = actualOutput.find(forbiddenMessage);
    const bool actualOutputIsEmpty = actualOutput.empty();
    REQUIRE(actualWinPosition == expectedWinPosition);
    REQUIRE(actualOutputIsEmpty == expectedOutputIsEmpty);
}

TEST_CASE("execute_EndOfInput_ReturnsWithoutInvalidInputOrWin", "[ui][regression]") {
    const bool expectedEndOfInput = true;
    const auto expectedInvalidPosition = std::string::npos;
    const auto expectedWinPosition = std::string::npos;
    const UserStatus expectedUserStatus = UserStatus::Playing;
    const std::string invalidMessage = "Invalid input";
    const std::string winMessage = "YOU WIN";
    const std::string inputText = "";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    ui.execute();

    const bool actualEndOfInput = input.eof();
    const std::string actualOutput = output.str();
    const auto actualInvalidPosition = actualOutput.find(invalidMessage);
    const auto actualWinPosition = actualOutput.find(winMessage);
    const UserStatus actualUserStatus = game.isUserWin();
    REQUIRE(actualEndOfInput == expectedEndOfInput);
    REQUIRE(actualInvalidPosition == expectedInvalidPosition);
    REQUIRE(actualWinPosition == expectedWinPosition);
    REQUIRE(actualUserStatus == expectedUserStatus);
}

TEST_CASE("execute_LastWinningLetterWithoutNewline_PrintsWin", "[ui][regression]") {
    const UserStatus expectedUserStatus = UserStatus::Won;
    const bool expectedEndOfInput = true;
    const std::string expectedMask = "level";
    const auto notFoundPosition = std::string::npos;
    const std::string winMessage = "YOU WIN";
    const std::string inputText = "l\ne\nv";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    ui.execute();

    const UserStatus actualUserStatus = game.isUserWin();
    const bool actualEndOfInput = input.eof();
    const std::string actualMask = game.getEncryptedWord();
    const std::string actualOutput = output.str();
    const auto actualWinPosition = actualOutput.find(winMessage);
    REQUIRE(actualUserStatus == expectedUserStatus);
    REQUIRE(actualEndOfInput == expectedEndOfInput);
    REQUIRE(actualMask == expectedMask);
    REQUIRE(actualWinPosition != notFoundPosition);
}

TEST_CASE("execute_EmptyDictionary_PrintsErrorWithoutReportingWin", "[ui][regression]") {
    const std::string expectedMessage = "Dictionary is empty";
    const auto notFoundPosition = std::string::npos;
    const auto expectedWinPosition = std::string::npos;
    const std::string winMessage = "YOU WIN";
    const std::string inputText = "";
    const std::string filename = "empty.txt";
    const std::string dictionaryPath = testDataPath(filename);
    HangmanGame game(dictionaryPath);
    std::istringstream input(inputText);
    std::ostringstream output;
    TerminalUI ui(game, input, output);

    ui.execute();

    const std::string actualOutput = output.str();
    const auto actualErrorPosition = actualOutput.find(expectedMessage);
    const auto actualWinPosition = actualOutput.find(winMessage);
    REQUIRE(actualErrorPosition != notFoundPosition);
    REQUIRE(actualWinPosition == expectedWinPosition);
}
