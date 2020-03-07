#include "../math/math.hpp"
#include "../math/heat_capacity.hpp"

#include <iostream>
#include <chrono>
#include <fstream>
int main(void) {


    Math::HeatCapacity c1 = [](auto t) {
        return exp(2*t);
    };

    Math::HeatCapacity c2 = [](auto t) {
        return 4*sin(t);
    };

    Math::HeatCapacitySmoother smoother(c1, c2, 1, 1, 0.2, Math::LINEAR);

    Math::DataVector vector;

    for (double x = 0; x < 2; x += 0.01) {
        auto value = smoother.GetValue(x);
        vector.push_back(value);
    }


    //std::ofstream file;
    //file.open("M:/linear1.txt", std::ios::out);
    //for (auto x : vector) {
    //    file << x << " ";
    //}
    //file.close();
    system("pause");
}