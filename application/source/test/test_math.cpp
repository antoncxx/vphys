#include "../math/math.hpp"
#include "../math/heat_capacity.hpp"
#include "../math/heat_conductivity.hpp"

#include <chrono>
#include <iostream>
#include <fstream>

bool ShouldStop(Math::DataVector& v1, Math::DataVector& v2, Math::Real epsilon = 1e-3) {
    Math::Real maxDifference = std::numeric_limits<Math::Real>::min();

    for (std::size_t i = 0; i < v1.size(); ++i) {
        auto diff = std::abs(v1[i] - v2[i]);
        if (maxDifference < diff) { maxDifference = diff; }
    }

    return maxDifference < epsilon;
}

inline Math::Real LeftBound(Math::Real time) {
    return std::sin(time) + 1;
}

inline Math::Real RightBound(Math::Real time) {
    return 10;
}

int main(void) {

    {
        using namespace Math;

        std::size_t xNodes = 100;

        const Real  pht = 5;      // phase transition temperature
        const Real  enthalpy = 100; // enthalpy

        const Real dx = 1e-1;
        const Real dt = 1e-2;
        const Real calculationTime = 1;

        std::vector<DataVector> data;

        {
            DataVector initialLayer = MakeDataVector(xNodes + 1);

            data.emplace_back(std::move(initialLayer));
        }

        auto c1 = [](auto t) { return 2; };
        auto c2 = [](auto t) { return 20; };

        HeatCapacitySmoother capc(c1, c2, pht, enthalpy, 1e-5, LINEAR); // smoothed heat capacity

        auto k1 = [](auto t) { return 0.2; };
        auto k2 = [](auto t) { return sin(6*t) + 5; };

        HeatConductivitySmoother cond(k1, k2, pht, 1e-5, LINEAR); // smoothed heat conductivity 

        Real time = 0;
        while (time < calculationTime) {

            DataVector prevLayer = data.back();

            std::size_t iteration = 0;
            do {
                DataVector nextLayer = MakeDataVector(xNodes + 1);

                const auto Aplus = [&](std::size_t i) -> Real {
                    return dt * cond.GetValue((prevLayer[i] + prevLayer[i + 1]) / 2) /
                        (dx * dx * capc.GetValue(prevLayer[i]));
                };

                const auto Aminus = [&](std::size_t i) -> Real {
                    return dt * cond.GetValue((prevLayer[i] + prevLayer[i - 1]) / 2) /
                        (dx * dx * capc.GetValue(prevLayer[i]));
                };

                const auto A = [&](std::size_t i) -> Real {
                    return -Aminus(i);
                };

                const auto B = [&](std::size_t i) -> Real {
                    return -Aplus(i);
                };

                const auto C = [&](std::size_t i) -> Real {
                    return 1 + Aplus(i) + Aminus(i);
                };

                const auto F = [&](std::size_t i) -> Real {
                    return prevLayer[i];
                };


                DataVector alpha = MakeDataVector(xNodes + 1);
                DataVector betta = MakeDataVector(xNodes + 1);

                alpha[1] = 0;
                betta[1] = LeftBound(time); // left boundary condition

                for (std::size_t i = 2; i < xNodes + 1; ++i) {
                    alpha[i] = -B(i - 1) / (A(i - 1) * alpha[i - 1] + C(i - 1));
                    betta[i] = (F(i - 1) - A(i - 1) * betta[i - 1]) / (A(i - 1) * alpha[i - 1] + C(i - 1));
                }

                nextLayer[xNodes] = RightBound(time); // right boundary condition

                for (std::size_t i = xNodes; i-- > 0;) {
                    nextLayer[i] = alpha[i + 1] * nextLayer[i + 1] + betta[i + 1];
                }

                bool shouldStop = ShouldStop(prevLayer, nextLayer);
                prevLayer = std::move(nextLayer);

                if (shouldStop) { break; }

            } while (++iteration < 10);

            data.emplace_back(std::move(prevLayer));
            time += dt;
        }

        //std::ofstream file("output.txt", std::ios::out);
        //
        //for (size_t i = 0; i < data.size(); i++)
        //{
        //    for (auto& x : data[i]) {
        //        file << x << " ";
        //    }
        //    file << std::endl;
        //}
        //
        //file.close();

    }


    std::system("pause");
}