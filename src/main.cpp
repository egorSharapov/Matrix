#include "matrix.hpp"
#include <iostream>

int main() {
    Matrix<int> m(5, 5, 19);
    m.dump(std::cout);

    Matrix<int> m2 = Matrix<int>::identity(5);
    m2.dump(std::cout);

    std::optional<int> trace = m2.trace();
    std::cout << "trace " << trace.value_or(999) << "\n";

    std::cout << "m[1][1] " << m[1][1] << "\n";

    m[1][1] = 10;
    std::cout << "m[1][1] " << m[1][1] << "\n";

    return 0;
}