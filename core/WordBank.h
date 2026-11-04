#ifndef E0676697_299C_4F13_8FA2_E9BC5A652973
#define E0676697_299C_4F13_8FA2_E9BC5A652973

#include "WorldBankResults.h"
#include "library/Result.h"
#include "GameInitStatus.h"
#include <string>
#include <algorithm>

class WordBank {
    private:
        std::string guessWord;
        std::string encryptedWord;
        std::string filePath;

    public:
        explicit WordBank(const std::string& filePath);

        const Result<std::string, RandomWorldError> chooseRandomWord() const;
        void encryptWord();
        GameInitStatus createWord();

        void openLetter(const char userLetter);

        const std::optional<int> getWordCount() const;
        const std::optional<int> getRandomNumber() const; 
        const std::string& getGuessedWord() const;
        const std::string& getEncryptedWord() const;
        void setWord(const std::string& word);

};

#endif /* E0676697_299C_4F13_8FA2_E9BC5A652973 */
