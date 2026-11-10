// Dictionary loading, random word selection and updates to the visible word mask.
#include "WordBank.h"
#include <random>
#include <vector>
#include <sstream>
#include <fstream>

WordBank::WordBank(const std::string& filePath)
    : filePath(filePath) {}

// Reveal every occurrence of the letter while keeping the mask the same length.
void WordBank::openLetter(const char userLetter) {
    for (size_t i = 0; i < guessWord.length(); ++i) {
        if (guessWord[i] == userLetter) {
            encryptedWord[i] = userLetter;
        }
    }
}

const std::optional<int> WordBank::getWordCount() const {

    std::ifstream file(filePath);

    if(!file.is_open()) {
        return std::nullopt;
    }

    int sumFileWord = 0;
    std::string line;
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string wordFromFile;

        while (ss >> wordFromFile) {
            sumFileWord++;
        }

    }

    return sumFileWord;
}

const std::optional<int> WordBank::getRandomNumber() const {
    auto totalWords = getWordCount();

    if (!totalWords.has_value() || totalWords == 0) {
        return std::nullopt;
    } 

    std::random_device rd;
    std::mt19937 gen(rd());
    // Word numbers start at one; chooseRandomWord converts them to vector indices.
    std::uniform_int_distribution<int> distrib(1, *totalWords);

    return distrib(gen);
}

const Result<std::string, RandomWorldError> WordBank::chooseRandomWord() const {
    std::ifstream file(filePath);

    if(!file.is_open()) {
        return Result<std::string, RandomWorldError>::failure(RandomWorldError::FileNotOpen);
    }

    std::vector<std::string> words; 
    std::string word;
    
    // Whitespace separates entries; normalize ASCII uppercase letters for guessing.
    while (file >> word) {
        for (char& letter : word) {
            if (letter >= 'A' && letter <= 'Z') {
                letter = static_cast<char>(letter - 'A' + 'a');
            }
        }

        words.push_back(word);
    }

    auto randomIndexWord = getRandomNumber();

    if (!randomIndexWord.has_value()) {
        return Result<std::string, RandomWorldError>::failure(RandomWorldError::IndexWordNotExist);;
    }

    return Result<std::string, RandomWorldError>::success(words[*randomIndexWord - 1]);

}

void WordBank::encryptWord() {
    std::string encryptedWord = "";
    for (size_t i = 0; i < guessWord.length(); i++) {
        encryptedWord += "*";
    }

    setWord(encryptedWord);
}

GameInitStatus WordBank::createWord() {
    auto randomWord = chooseRandomWord();   
    if (!randomWord.hasValue()) {
        // Discard the previous word and mask if the new round cannot be initialized.
        guessWord.clear();
        encryptedWord.clear();

        switch (randomWord.error()) {
            case RandomWorldError::IndexWordNotExist:
                return GameInitStatus::IndexWordNotExist;
            
            case RandomWorldError::FileNotOpen:
                return GameInitStatus::FileNotOpen;
        }
    }

    guessWord = randomWord.value(); 
    encryptedWord = std::string(guessWord.length(), '*');

    return GameInitStatus::Success;
}

const std::string& WordBank::getGuessedWord() const {
    return guessWord;
}

const std::string& WordBank::getEncryptedWord() const {
    return encryptedWord;
}

void WordBank::setWord(const std::string& word) {
    encryptedWord = word;
}
