#include "core/HangmanGame.h"
#include "app/HangmanApp.h"


// Creates the game and application, then delegates control to the application.
int main(int argc, char* argv[]) {

    HangmanGame game("hangman_data/words.txt");
    HangmanApp app(game);
    app.run(argc, argv);

    return 0;
}