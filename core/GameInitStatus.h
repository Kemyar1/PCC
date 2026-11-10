#ifndef D1CF020F_A7BF_4E9F_A298_9CA747C78488
#define D1CF020F_A7BF_4E9F_A298_9CA747C78488

// Initialization results and round states shared by the game logic and the UI.
enum class GameInitStatus {
    FileNotOpen,
    IndexWordNotExist,
    Success
};

enum class UserStatus {
    Playing,
    Won,
    Lost
};

#endif /* D1CF020F_A7BF_4E9F_A298_9CA747C78488 */
