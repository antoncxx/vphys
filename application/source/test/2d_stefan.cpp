#include <fstream>
#include <iostream>

#include "../math/heat_conductivity.hpp"
#include "../math/heat_capacity.hpp"

#define MAX_ITERATION 10

Math::Real LeftBound(Math::Real time) {
    return 0;
}

Math::Real RightBound(Math::Real time) {
    return 0;
}

Math::Real UpperBound(Math::Real time) {
    return 0;
}

Math::Real LowerBound(Math::Real time) {
    return 10;
}

bool ShouldStop(const Math::DataMatrix& first, const Math::DataMatrix& second, Math::Real epsilon = 1e-3) {
    Math::Real maxDifference = std::numeric_limits<Math::Real>::min();

    for (std::size_t i = 0; i < first.size(); ++i) {
        for (std::size_t j = 0; j < first[i].size(); ++j) {
            auto diff = std::abs(first[i][j] - second[i][j]);
            if (maxDifference < diff) { maxDifference = diff; }
        }
    }

    return maxDifference < epsilon;
}

int main(int argc, char** argv) {
    using namespace Math;

    Real L1 = 10; // width of computational domain
    Real L2 = 10;   // height of computational domain
    
    const Real dx = 0.1; 
    const Real dy = 0.1; 

    const std::size_t xNodes = static_cast<std::size_t>(L1 / dx);
    const std::size_t yNodes = static_cast<std::size_t>(L2 / dy);

    const Real calculationTime = 1;
    const Real dt = 1e-1;

    const Real phaseTrastionTemperature = 5;
    const Real enthalpy = 10;
    const Real delta = 1e-1;

    auto c1 = [](auto t) -> Real { return 2; };
    auto c2 = [](auto t) -> Real { return 20; };

    HeatCapacitySmoother capacity(c1, c2, phaseTrastionTemperature, enthalpy, delta, LINEAR); // smoothed heat capacity

    auto k1 = [](auto t) -> Real { return 0.2; };
    auto k2 = [](auto t) -> Real { return sin(6 * t) + 5; };

    HeatConductivitySmoother conductivity(k1, k2, phaseTrastionTemperature, delta, LINEAR); // smoothed heat conductivity 


    // vector to store data
    std::vector<DataMatrix> data;

    // initial conditions
    {
        DataMatrix initialLayer = MakeDataMatrix(xNodes + 1, yNodes + 1);
        data.emplace_back(std::move(initialLayer));
    }

    Real time = 0; // current time
    while (time < calculationTime) {

        DataMatrix prevIteration = data.back();
        std::size_t iteration = 0;

        do {

            DataMatrix nextIteration = MakeDataMatrix(xNodes + 1, yNodes + 1);
            
            // helper coeficients definitions

            const auto OmegaPlusX = [&](std::size_t i, std::size_t j) -> Real {
                return dt * conductivity.GetValue((prevIteration[i][j] + prevIteration[i + 1][j]) / 2) /
                    (dx * dx * capacity.GetValue(prevIteration[i][j]));
            };

            const auto OmegaMinusX = [&](std::size_t i, std::size_t j) -> Real {
                return dt * conductivity.GetValue((prevIteration[i][j] + prevIteration[i - 1][j]) / 2) /
                    (dx * dx * capacity.GetValue(prevIteration[i][j]));
            };

            const auto OmegaPlusY = [&](std::size_t i, std::size_t j) -> Real {
                return dt * conductivity.GetValue((prevIteration[i][j] + prevIteration[i][j + 1]) / 2) /
                    (dy * dy * capacity.GetValue(prevIteration[i][j]));
            };

            const auto OmegaMinusY = [&](std::size_t i, std::size_t j) -> Real {
                return dt * conductivity.GetValue((prevIteration[i][j] + prevIteration[i][j - 1]) / 2) /
                    (dy * dy * capacity.GetValue(prevIteration[i][j]));
            };

            // solve a system of one-dimensional problems along X
            for (std::size_t k = 0; k < yNodes + 1; ++k) {
                // coeffisients
                const auto A = [&](std::size_t index) -> Real {
                    return -OmegaMinusX(index, k);
                };

                const auto B = [&](std::size_t index) -> Real {
                    return -OmegaPlusX(index, k);
                };

                const auto C = [&](std::size_t index) -> Real {
                    return 1 + OmegaPlusX(index, k) + OmegaMinusX(index, k);
                };

                const auto F = [&](std::size_t index) -> Real {
                    return data.back()[index][k];
                };

                auto alpha = MakeDataVector(xNodes + 1);
                auto betta = MakeDataVector(xNodes + 1);

                alpha[1] = 0;
                betta[1] = LeftBound(time);

                for (std::size_t i = 2; i < xNodes + 1; ++i) {
                    alpha[i] = -B(i - 1) / (A(i - 1) * alpha[i - 1] + C(i - 1));
                    betta[i] = (F(i - 1) - A(i - 1) * betta[i - 1]) / (A(i - 1) * alpha[i - 1] + C(i - 1));
                }

                nextIteration[xNodes][k] = RightBound(time);

                for (std::size_t i = xNodes; i-- > 0;) {
                    nextIteration[i][k] = alpha[i + 1] * nextIteration[i + 1][k] + betta[i + 1];
                }
            }
            
            // solve a system of one-dimensional problems along Y
            for (std::size_t k = 0; k < xNodes + 1; ++k) {
                // coeffisients
                const auto A = [&](std::size_t index) -> Real {
                    return -OmegaMinusY(k, index);
                };

                const auto B = [&](std::size_t index) -> Real {
                    return -OmegaPlusY(k, index);
                };

                const auto C = [&](std::size_t index) -> Real {
                    return 1 + OmegaPlusY(k, index) + OmegaMinusY(k, index);
                };

                const auto F = [&](std::size_t index) -> Real {
                    return nextIteration[k][index];
                };

                auto alpha = MakeDataVector(yNodes + 1);
                auto betta = MakeDataVector(yNodes + 1);

                alpha[1] = 0;
                betta[1] = k < 10 ? LowerBound(time) : 0;

                for (std::size_t i = 2; i < yNodes + 1; ++i) {
                    alpha[i] = -B(i - 1) / (A(i - 1) * alpha[i - 1] + C(i - 1));
                    betta[i] = (F(i - 1) - A(i - 1) * betta[i - 1]) / (A(i - 1) * alpha[i - 1] + C(i - 1));
                }

                nextIteration[k][yNodes] = UpperBound(time);

                for (std::size_t i = yNodes; i-- > 0;) {
                    nextIteration[k][i] = alpha[i + 1] * nextIteration[k][i + 1] + betta[i + 1];
                }
            }

            bool shouldStop = ShouldStop(nextIteration, prevIteration);
            prevIteration = std::move(nextIteration);

            if (shouldStop) {
                break;
            }

        } while (iteration++ < MAX_ITERATION);

        data.emplace_back(std::move(prevIteration));
        time += dt;
    }

    //std::ofstream file("output2d.txt", std::ios::out);
    //for (auto& row : data.back()) {
    //    for (auto& value : row) {
    //        file << value << " ";
    //    }

    //    file << std::endl;
    //}
    //file.close();



    system("pause");
}