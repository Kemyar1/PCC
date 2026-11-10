#ifndef AFBC2BBB_8A6A_4BD3_A668_A00BF3F015A2
#define AFBC2BBB_8A6A_4BD3_A668_A00BF3F015A2
// Distinguishes invalid/repeated guesses from a valid letter missing from the word.
enum class CheckingLetterError{
    LetterWasUsed,
    InvalidType
};

enum class GuessLetterError {
    LetterWasUsed,
    InvalidType,
    LetterNotFound,
};



#endif /* AFBC2BBB_8A6A_4BD3_A668_A00BF3F015A2 */
