#include <iostream>

#include "../math/math.hpp"

int main(void) {
    Math::DataVector f = { 1,5,6 };

    Math::DataMatrix matrix = {
        {1,5,7},
        {2,4,6},
        {3,5,7}
    };

    for (auto x : Math::SolveEquasions(matrix, f, 3)) {
        std::cout << x << std::endl;
    }

    system("pause");
}