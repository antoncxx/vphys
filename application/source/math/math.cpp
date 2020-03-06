#include "math.hpp"

namespace Math {
    DataVector MakeDataVector(std::size_t xSize, Real defaulValue) {
        return DataVector(xSize, defaulValue);
    }

    DataMatrix MakeDataMatrix(std::size_t xSize, std::size_t ySize, Real defaulValue) {
        return DataMatrix(xSize, MakeDataVector(ySize, defaulValue));
    }

    DataCube MakeDataCube(std::size_t xSize, std::size_t ySize, std::size_t zSize, Real defaulValue) {
        return DataCube(xSize, MakeDataMatrix(ySize, zSize, defaulValue));
    }
}