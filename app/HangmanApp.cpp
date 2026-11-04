#include "HangmanApp.h"
#include "../core/HangmanGame.h"

#include <memory>
#include <iostream>


HangmanApp::HangmanApp(HangmanGame &game) 
    : commandUI(std::make_unique<TerminalUI>(game)) {}


std::optional<int> HangmanApp::readInt(const std::string &prompt) {
    std::cout << prompt;

    int value;
    std::cin >> value; 

    if (std::cin.fail()) {
        if (std::cin.eof() || std::cin.bad()) {
            return std::nullopt;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        return std::nullopt;
    }

    std::string remainingLine;
    std::getline(std::cin, remainingLine);

    return value;
}

void HangmanApp::run(int argc, char* argv[]) {

    for (int index = 1; index < argc; ++index) {
        const std::string argument = argv[index];

        if (argument == "--help") {
            commandUI->handlePrintHelp();
            return;
        }
    }

    bool running = true;
    while (running) {
        printMainMenu();

        auto choose = readInt("Choose option: ");

        if (!choose.has_value()) {
            if (std::cin.eof() || std::cin.bad()) {
                return;
            }

            std::cout << "Invalid option\n";
            continue;
        }


        if (*choose == 0) {
            running = false;
            continue;
        } else if (*choose == 1) {
            commandUI->execute();
            if (std::cin.eof() || std::cin.bad()) {
                return;
            }
        } else {
            std::cout << "Invalid option\n";
        }

    }
}

void HangmanApp::printMainMenu() {
    std::cout << "====================================\n";
        std::cout << "          WELCOME TO HANGMAN        \n";
        std::cout << "====================================\n";
        std::cout << "Guess the secret word letter by letter.\n\n";

        std::cout << "1) Start\n"
                  << "0) Exit\n";
}
