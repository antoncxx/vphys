#include "../math/math.hpp"

#include <chrono>
#include <iostream>

int main(void) {
    // test vector
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto vector = Math::MakeDataVector(300);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Vector creation : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << std::endl;
    }

    // test matrix
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto matrix = Math::MakeDataMatrix(300, 300);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Matrix creation : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << std::endl;
    }

    //test cube
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto cube = Math::MakeDataCube(300, 300, 300);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Cube creation : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << std::endl;
    }

    // test basic operation
    {
        auto cube1 = Math::MakeDataCube(300, 300, 300);
        auto cube2 = Math::MakeDataCube(300, 300, 300);

        auto start = std::chrono::high_resolution_clock::now();

        Math::DataCube cube3 = cube2;

        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Cube copy time : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << std::endl;
        // ============================================================

        start = std::chrono::high_resolution_clock::now();

        Math::DataCube cube4 = std::move(cube3);

        end = std::chrono::high_resolution_clock::now();

        std::cout << "Cube move time : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << std::endl;
        // ============================================================

        start = std::chrono::high_resolution_clock::now();

        for (auto& matrix : cube4) {
            for (auto& vector : matrix) {
                for (auto& elem : vector) {
                    elem = elem * 2;
                }
            }
        }

        end = std::chrono::high_resolution_clock::now();

        std::cout << "Cube iteration over elements : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << std::endl;
    }


    std::system("pause");
}