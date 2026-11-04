#include "core/HangmanGame.h"
#include "app/HangmanApp.h"
#include "test.h"
#include <iostream>

// void printArray(const double* arr, const size_t size) {
//     double* pend = const_cast<double*>(arr) + size;
//     for (double* p = const_cast<double *>(arr); p < pend; p++) {
//         std::cout << *p << " ";
//     }
//     std::cout << std::endl;
// }

// void copyArray(const double* from, double* to, const size_t size) {
//     for (size_t i = 0; i < size; i++) {
//         to[i] = from[i];
//     }
// }

// void resizeArray(double* arr, const size_t size, const size_t newSize) {
//     double* newArray = new double[newSize];
//     size_t minSize = size < newSize ? size : newSize;
//     copyArray(arr, newArray, minSize);
//     delete[] arr;
//     arr = newArray;
// }

int main(int argc, char* argv[]) {
// int main() {

    HangmanGame game("hangman_data/words.txt");
    HangmanApp app(game);
    app.run(argc, argv);

    // std::vector<int> first = {1, 2, 3, 4};
    // std::vector<int> second = first;
    // second.push_back(0);

    // run();

    



    return 0;
}