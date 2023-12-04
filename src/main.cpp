#include "matrix.hpp"
#include <cassert>
#include <iostream>

/*
consteval std::optional<int> calc_det() {
    Matrix<int> m = {{1, 2}, {3, 4}};
    return m.det();
}
*/

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
    int determinant = m.det().value_or(999);
    std::cout << determinant;

    return 0;
}