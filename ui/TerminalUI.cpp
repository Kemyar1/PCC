// Terminal interaction and presentation, including colored feedback and hangman stages.
#include "TerminalUI.h"
#include <iostream>

constexpr const char* UI_RESET = "\x1B[0m";
constexpr const char* UI_GREEN = "\x1B[32m";
constexpr const char* UI_RED = "\x1B[31m";
constexpr const char* UI_ORANGE = "\x1B[38;5;208m";
constexpr const char* UI_CYAN = "\x1B[36m";

TerminalUI::TerminalUI(HangmanGame& game, std::istream& input, std::ostream& output)
    : game(game), input(input), output(output) {}


// Read the whole line to reject empty or multi-character guesses as a single input.
std::optional<char> TerminalUI::readChar(const std::string &prompt) const {
    output << prompt;

    std::string value;
    std::getline(input, value); 

    if (input.fail()) {
        if (input.eof() || input.bad()) {
            return std::nullopt;
        }

        input.clear();
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        return std::nullopt;
    }

    if (value.length() != 1) {
        return std::nullopt;
    }

    return value[0];
}


void TerminalUI::execute() {
    auto resultGame = game.resetGame();
    
    // Stop the round immediately if the dictionary could not provide a word.
    switch (resultGame) {
        case GameInitStatus::FileNotOpen:
            output << UI_ORANGE << "ERROR: File was not opened\n" << UI_RESET;
            return;

        case GameInitStatus::IndexWordNotExist:
            output << UI_ORANGE << "ERROR: Dictionary is empty\n" << UI_RESET;
            return;

        case GameInitStatus::Success:
            break;
    }

    while (true) {
        // Check the outcome before EOF: the last input may already have won the game.
        auto result = handlePrintUserStatus();
            switch (result) {
                case UserStatus::Playing:
                    break;

                case UserStatus::Lost:
                case UserStatus::Won:
                    return;
            }

        if (input.eof() || input.bad()) {
            return;
        }

        handleViewWord();
        handleViewWrongAttempts();
        handleTypeLetter();
    }
}

void TerminalUI::handlePrintHelp() {
    const std::string accent = "\x1B[36m";
    const std::string muted = "\x1B[90m";
    const std::string reset = "\x1B[0m";

    output
        << muted
        << "\n  ------------------------------------------------------\n"
        << reset
        << accent
        << "  HANGMAN  /  HELP\n"
        << reset
        << "  Guess the hidden word, one letter at a time.\n"
        << muted
        << "  ------------------------------------------------------\n"
        << reset

        << accent
        << "\n  USAGE\n"
        << reset
        << "    HangmanApp           Start the application.\n"
        << "    HangmanApp --help    Show this help and exit.\n"

        << accent
        << "\n  MAIN MENU\n"
        << reset
        << "    1    Start a new round.\n"
        << "    0    Exit the application.\n"

        << accent
        << "\n  HOW TO PLAY\n"
        << reset
        << "    Enter one lowercase English letter (a-z),\n"
        << "    then press Enter to submit your guess.\n\n"
        << "    A correct guess reveals all matching letters.\n"
        << "    A wrong guess adds one part to the hangman.\n"
        << "    Repeated letters and invalid input do not\n"
        << "    increase the number of wrong guesses.\n"

        << accent
        << "\n  ROUND RESULT\n"
        << reset
        << "    WIN     Reveal the entire word.\n"
        << "    LOSE    Make 6 wrong guesses.\n\n"
        << "    After the round, you return to the main menu.\n"

        << muted
        << "\n  ------------------------------------------------------\n"
        << reset
        << '\n';
}


const Result<bool, GuessLetterError> TerminalUI::handleTypeLetter() const {
    auto letter = readChar("  Enter a letter: ");

    if (!letter.has_value()) {
        if (!input.eof() && !input.bad()) {
            output << UI_ORANGE
                   << "\n  [!] Invalid input. Try a letter.\n\n"
                   << UI_RESET;
        }
        return Result<bool, GuessLetterError>::failure(GuessLetterError::InvalidType);
    }

    // Translate core results into colored feedback; the game updates its own state.
    auto result = game.guessLetter(*letter);
    if (!result.hasValue()) {
        switch (result.error()) {
            case GuessLetterError::LetterNotFound:
                output << UI_RED
                       << "\n  [-] No new letter opened. Check the board.\n\n"
                       << UI_RESET;
                return Result<bool, GuessLetterError>::failure(GuessLetterError::LetterNotFound);

            case GuessLetterError::LetterWasUsed:
                output << UI_ORANGE
                       << "\n [!] You have already used this letter. Try another one.\n\n"
                       << UI_RESET;
                return Result<bool, GuessLetterError>::failure(GuessLetterError::LetterWasUsed);
            case GuessLetterError::InvalidType:
                output << UI_ORANGE
                       << "\n [!] Invalid input. Try a letter.\n\n"
                       << UI_RESET;
                return Result<bool, GuessLetterError>::failure(GuessLetterError::InvalidType);
        }
    }

    output << UI_GREEN
           << "\n  [+] Good guess! The letter is in the word.\n\n"
           << UI_RESET;
    return Result<bool, GuessLetterError>::success(true);
    
}

UserStatus TerminalUI::handlePrintUserStatus() {
    auto userStatus = game.isUserWin();

    switch (userStatus) {
        case UserStatus::Playing:
            break;

        case UserStatus::Won:
            output << "\n========================================\n"
                      << UI_GREEN << "  YOU WIN!\n" << UI_RESET
                      << "  Word: " << UI_CYAN << game.getGuessedWord()
                      << UI_RESET << '\n'
                      << "========================================\n\n";
            break;

        case UserStatus::Lost:
            output << "\n=========== FINAL DRAWING ==============\n";
            drawHangmanStage(game.getWrongAttempts());
            output << UI_RED << "\n  GAME OVER\n" << UI_RESET
                      << "  The word was: " << UI_CYAN << game.getGuessedWord()
                      << UI_RESET << '\n'
                      << "========================================\n\n";
            break;
    }

    return userStatus;

}

void TerminalUI::handleViewWrongAttempts() {
    output << "  Wrong guesses: " << game.getWrongAttempts() << " / 6\n"
              << "----------------------------------------\n";
    drawHangmanStage(game.getWrongAttempts());
    output << "----------------------------------------\n";
}

void TerminalUI::handleCreateWord() {
    game.createWord();
}

void TerminalUI::handleViewWord() {
    output << "\n+--------------------------------------+\n"
              << "|               THE WORD               |\n"
              << "+--------------------------------------+\n"
              << "\n  " << UI_CYAN << game.getEncryptedWord()
              << UI_RESET << "\n\n";
}

// Select the drawing by mistake count; the individual stages only print ASCII art.
void TerminalUI::drawHangmanStage(int mistakes) {
    if (mistakes <= 0) {
        drawStage0();
    } else if (mistakes == 1) {
        drawStage1();
    } else if (mistakes == 2) {
        drawStage2();
    } else if (mistakes == 3) {
        drawStage3();
    } else if (mistakes == 4) {
        drawStage4();
    } else if (mistakes == 5) {
        drawStage5();
    } else {
        drawStage6();
    }
}

void TerminalUI::drawStage0() {
    output << R"(     +-------+
     |       |
             |
             |
             |
             |
     =========
)";
}

void TerminalUI::drawStage1() {
    output << R"(     +-------+
     |       |
     O       |
             |
             |
             |
     =========
)";
}

void TerminalUI::drawStage2() {
    output << R"(     +-------+
     |       |
     O       |
     |       |
             |
             |
     =========
)";
}

void TerminalUI::drawStage3() {
    output << R"(     +-------+
     |       |
     O       |
    /|       |
             |
             |
     =========
)";
}

void TerminalUI::drawStage4() {
    output << R"(     +-------+
     |       |
     O       |
    /|\      |
             |
             |
     =========
)";
}

void TerminalUI::drawStage5() {
    output << R"(     +-------+
     |       |
     O       |
    /|\      |
    /        |
             |
     =========
)";
}

void TerminalUI::drawStage6() {
    output << R"(     +-------+
     |       |
     O       |
    /|\      |
    / \      |
             |
     =========
)";
}