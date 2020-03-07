#pragma once

#include <vector>

namespace Math {

    using Real       = double; 
    using DataVector = std::vector<Real>;

    using DataMatrix = std::vector<DataVector>;
    using DataCube   = std::vector<DataMatrix>;

    DataVector MakeDataVector(std::size_t xSize, Real defaulValue = Real());
    DataMatrix MakeDataMatrix(std::size_t xSize, std::size_t ySize, Real defaulValue = Real());
    DataCube   MakeDataCube(std::size_t xSize, std::size_t ySize, std::size_t zSize, Real defaulValue = Real());

    template<class F, class T>
    auto Integrate(F&& func, T a, T b, T step) {
        auto steps = static_cast<std::uint64_t>((b - a) / step);
        auto result = T(0);
        for (std::uint64_t i = 0; i < steps; ++i) {
            result += (func(a + i * step) + func(a + (i + 1) * step)) / 2 * step;
        }
        return result;
    }
}