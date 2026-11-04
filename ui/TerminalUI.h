#ifndef D9BCCD1A_A839_4B92_BB43_641BD2CE64E3
#define D9BCCD1A_A839_4B92_BB43_641BD2CE64E3

#include "../core/HangmanGame.h"
#include <optional>
#include <string>
#include "../core/library/Result.h"
#include <iostream>

class TerminalUI {
    private:
        HangmanGame &game;
        std::istream& input;
        std::ostream& output;

        std::optional<int> readInt(const std::string& prompt) const;
        std::optional<char> readChar(const std::string& prompt) const;
        std::string readLine(const std::string& prompt) const;

    public:
        explicit TerminalUI(HangmanGame& game, std::istream& input = std::cin, std::ostream& output = std::cout);
        void execute();
        void handlePrintHelp();

        void handleViewWord();
        const Result<bool, GuessLetterError> handleTypeLetter() const;
        UserStatus handlePrintUserStatus();
        void exitGame();

        void handleViewWrongAttempts();

        void handleCreateWord();

        void drawHangmanStage(int mistakes);
        void drawStage0();
        void drawStage1();
        void drawStage2();
        void drawStage3();
        void drawStage4();
        void drawStage5();
        void drawStage6();

        

};

#endif /* D9BCCD1A_A839_4B92_BB43_641BD2CE64E3 */
