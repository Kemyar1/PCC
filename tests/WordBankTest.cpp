#include "../core/WordBank.h"
#include "TestData.h"
#include "vendor/catch.hpp"

#include <algorithm>
#include <string>
#include <vector>

TEST_CASE("getWordCount_MissingFile_ReturnsNoValue", "[wordbank]") {
    const bool expectedHasValue = false;
    const std::string filename = "does_not_exist.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    auto result = bank.getWordCount();

    const bool actualHasValue = result.has_value();
    REQUIRE(actualHasValue == expectedHasValue);
}

TEST_CASE("getWordCount_EmptyAndWhitespaceFiles_ReturnsZero", "[wordbank]") {
    const int expectedWordCount = 0;
    const bool expectedHasValue = true;
    const std::string emptyFilename = "empty.txt";
    const std::string whitespaceFilename = "whitespace.txt";
    std::string filename;
    SECTION("Empty file") { filename = emptyFilename; }
    SECTION("Whitespace only") { filename = whitespaceFilename; }
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    auto result = bank.getWordCount();

    const bool actualHasValue = result.has_value();
    REQUIRE(actualHasValue == expectedHasValue);

    const int actualWordCount = *result;
    REQUIRE(actualWordCount == expectedWordCount);
}

TEST_CASE("getWordCount_SeveralWordsAcrossLines_CountsAllWords", "[wordbank]") {
    const int expectedWordCount = 4;
    const bool expectedHasValue = true;
    const std::string filename = "multiple.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    auto result = bank.getWordCount();

    const bool actualHasValue = result.has_value();
    REQUIRE(actualHasValue == expectedHasValue);

    const int actualWordCount = *result;
    REQUIRE(actualWordCount == expectedWordCount);
}

TEST_CASE("getRandomNumber_SingleWord_ReturnsOne", "[wordbank]") {
    const int expectedNumber = 1;
    const bool expectedHasValue = true;
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    auto result = bank.getRandomNumber();

    const bool actualHasValue = result.has_value();
    REQUIRE(actualHasValue == expectedHasValue);

    const int actualNumber = *result;
    REQUIRE(actualNumber == expectedNumber);
}

TEST_CASE("getRandomNumber_MissingOrEmptyFile_ReturnsNoValue", "[wordbank]") {
    const bool expectedHasValue = false;
    const std::string missingFilename = "does_not_exist.txt";
    const std::string emptyFilename = "empty.txt";
    const std::string whitespaceFilename = "whitespace.txt";
    std::string filename;
    SECTION("Missing file") { filename = missingFilename; }
    SECTION("Empty file") { filename = emptyFilename; }
    SECTION("Whitespace only") { filename = whitespaceFilename; }
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    auto result = bank.getRandomNumber();

    const bool actualHasValue = result.has_value();
    REQUIRE(actualHasValue == expectedHasValue);
}

TEST_CASE("getRandomNumber_SeveralWords_StaysWithinWordCount", "[wordbank]") {
    const int expectedMinimum = 1;
    const int expectedMaximum = 4;
    const bool expectedHasValue = true;
    const int iterationCount = 20;
    const std::string filename = "multiple.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    for (int iteration = 0; iteration < iterationCount; ++iteration) {
        auto result = bank.getRandomNumber();

        const bool actualHasValue = result.has_value();
        REQUIRE(actualHasValue == expectedHasValue);

        const int actualNumber = *result;
        REQUIRE(actualNumber >= expectedMinimum);
        REQUIRE(actualNumber <= expectedMaximum);
    }
}

TEST_CASE("chooseRandomWord_SingleWord_ReturnsThatWord", "[wordbank]") {
    const std::string expectedWord = "level";
    const bool expectedHasValue = true;
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    auto result = bank.chooseRandomWord();

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const std::string actualWord = result.value();
    REQUIRE(actualWord == expectedWord);
}

TEST_CASE("chooseRandomWord_MissingFile_ReturnsFileNotOpen", "[wordbank]") {
    const RandomWorldError expectedError = RandomWorldError::FileNotOpen;
    const bool expectedHasValue = false;
    const std::string filename = "does_not_exist.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    auto result = bank.chooseRandomWord();

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const RandomWorldError actualError = result.error();
    REQUIRE(actualError == expectedError);
}

TEST_CASE("chooseRandomWord_EmptyOrWhitespaceFile_ReturnsIndexWordNotExist", "[wordbank]") {
    const RandomWorldError expectedError = RandomWorldError::IndexWordNotExist;
    const bool expectedHasValue = false;
    const std::string emptyFilename = "empty.txt";
    const std::string whitespaceFilename = "whitespace.txt";
    std::string filename;
    SECTION("Empty file") { filename = emptyFilename; }
    SECTION("Whitespace only") { filename = whitespaceFilename; }
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    auto result = bank.chooseRandomWord();

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const RandomWorldError actualError = result.error();
    REQUIRE(actualError == expectedError);
}

TEST_CASE("chooseRandomWord_SeveralWords_ReturnsDictionaryMember", "[wordbank]") {
    const std::vector<std::string> expectedWords = {"level", "apple", "banana", "pear"};
    const bool expectedHasValue = true;
    const bool expectedWordIsListed = true;
    const std::string filename = "multiple.txt";
    const std::string dictionaryPath = testDataPath(filename);
    const auto wordsBegin = expectedWords.begin();
    const auto wordsEnd = expectedWords.end();
    WordBank bank(dictionaryPath);

    auto result = bank.chooseRandomWord();

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const std::string actualWord = result.value();
    const auto matchingWord = std::find(wordsBegin, wordsEnd, actualWord);
    const bool actualWordIsListed = matchingWord != wordsEnd;
    REQUIRE(actualWordIsListed == expectedWordIsListed);
}

TEST_CASE("createWord_ValidDictionary_SetsWordAndMask", "[wordbank]") {
    const GameInitStatus expectedStatus = GameInitStatus::Success;
    const std::string expectedWord = "level";
    const std::string expectedMask = "*****";
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    const GameInitStatus actualStatus = bank.createWord();
    REQUIRE(actualStatus == expectedStatus);

    const std::string actualWord = bank.getGuessedWord();
    const std::string actualMask = bank.getEncryptedWord();
    REQUIRE(actualWord == expectedWord);
    REQUIRE(actualMask == expectedMask);
}

TEST_CASE("createWord_MissingFile_ReturnsFileNotOpen", "[wordbank][regression]") {
    const GameInitStatus expectedStatus = GameInitStatus::FileNotOpen;
    const std::string filename = "does_not_exist.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    const GameInitStatus actualStatus = bank.createWord();

    REQUIRE(actualStatus == expectedStatus);
}

TEST_CASE("createWord_EmptyDictionary_ReturnsIndexWordNotExist", "[wordbank]") {
    const GameInitStatus expectedStatus = GameInitStatus::IndexWordNotExist;
    const std::string filename = "empty.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    const GameInitStatus actualStatus = bank.createWord();

    REQUIRE(actualStatus == expectedStatus);
}

TEST_CASE("openLetter_RepeatedLetter_RevealsAllOccurrences", "[wordbank]") {
    const std::string expectedMask = "l***l";
    const std::string expectedWord = "level";
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const char inputLetter = 'l';
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);
    const GameInitStatus actualInitStatus = bank.createWord();
    REQUIRE(actualInitStatus == expectedInitStatus);

    bank.openLetter(inputLetter);

    const std::string actualMask = bank.getEncryptedWord();
    const std::string actualWord = bank.getGuessedWord();
    REQUIRE(actualMask == expectedMask);
    REQUIRE(actualWord == expectedWord);
}

TEST_CASE("openLetter_AbsentLetter_LeavesMaskUnchanged", "[wordbank]") {
    const std::string expectedMask = "*****";
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const char inputLetter = 'a';
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);
    const GameInitStatus actualInitStatus = bank.createWord();
    REQUIRE(actualInitStatus == expectedInitStatus);

    bank.openLetter(inputLetter);

    const std::string actualMask = bank.getEncryptedWord();
    REQUIRE(actualMask == expectedMask);
}

TEST_CASE("encryptWord_CreatedWord_PreservesOneMaskCharacterPerLetter", "[wordbank][regression]") {
    const std::string expectedMask = "*****";
    const GameInitStatus expectedInitStatus = GameInitStatus::Success;
    const std::string filename = "words.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);
    const GameInitStatus actualInitStatus = bank.createWord();
    REQUIRE(actualInitStatus == expectedInitStatus);

    bank.encryptWord();

    const std::string actualMask = bank.getEncryptedWord();
    REQUIRE(actualMask == expectedMask);
}

TEST_CASE("chooseRandomWord_MixedCaseDictionary_ReturnsLowercaseWord", "[wordbank][regression]") {
    const std::string expectedWord = "level";
    const bool expectedHasValue = true;
    const std::string filename = "mixed_case.txt";
    const std::string dictionaryPath = testDataPath(filename);
    WordBank bank(dictionaryPath);

    auto result = bank.chooseRandomWord();

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const std::string actualWord = result.value();
    REQUIRE(actualWord == expectedWord);
}
