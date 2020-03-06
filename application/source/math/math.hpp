#pragma once

#include <vector>

namespace Math {

    using Real       = float; // use float for now instead of double to save some memory
    using DataVector = std::vector<Real>;

    using DataMatrix = std::vector<DataVector>;
    using DataCube   = std::vector<DataMatrix>;

    DataVector MakeDataVector(std::size_t xSize, Real defaulValue = Real());
    DataMatrix MakeDataMatrix(std::size_t xSize, std::size_t ySize, Real defaulValue = Real());
    DataCube   MakeDataCube(std::size_t xSize, std::size_t ySize, std::size_t zSize, Real defaulValue = Real());
}