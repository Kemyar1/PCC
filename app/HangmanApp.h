#ifndef F23D226B_60F6_43FA_B1E4_8ADFD776000A
#define F23D226B_60F6_43FA_B1E4_8ADFD776000A

#include "../core/HangmanGame.h"
#include "../ui/TerminalUI.h"
#include <algorithm>
#include <string>
#include <memory>

class HangmanApp {
    private:
        std::unique_ptr<TerminalUI> commandUI;

        static std::optional<int> readInt(const std::string& prompt);

    public:
        explicit HangmanApp(HangmanGame &game);

        void run(int argc, char* argv[]);
        void printMainMenu();
};

#endif /* F23D226B_60F6_43FA_B1E4_8ADFD776000A */
