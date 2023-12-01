#include "matrix.hpp"
#include <iostream>

int main() {
    size_t size = 0;
    int cell = 0;

    std::cin >> size;
    Matrix<int> m(size, size);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cin >> cell;
            m[i][j] = cell;
        }
    }
    long double determinant = m.det().value_or(0);
    std::cout << std::round(determinant);
    
    return 0;
}