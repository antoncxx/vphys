#include "math.hpp"
#include <stdexcept>

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

    DataVector SolveEquasions(DataMatrix& coefficients, DataVector& values, std::size_t size, Real epsilon) {
        DataVector x = MakeDataVector(size);

        for (std::size_t k = 0; k < size; ++k) {
            Real max = std::abs(coefficients[k][k]);
            std::size_t index = k;

            for (std::size_t i = k + 1; i < size; ++i) {
                if (std::abs(coefficients[i][k]) > max) {
                    max = std::abs(coefficients[i][k]);
                    index = i;
                }
            }

            if (max < epsilon) {
                throw std::logic_error("The solution cannot be obtained because of the null column");
            }

            for (std::size_t j = 0; j < size; ++j) {
                std::swap(coefficients[k][j], coefficients[index][j]);
            }

            std::swap(values[k], values[index]);

            for (std::size_t i = k; i < size; ++i) {
                Real temp = coefficients[i][k];

                if (std::abs(temp) < epsilon) {
                    continue;
                }

                for (std::size_t j = 0; j < size; ++j) {
                    coefficients[i][j] = coefficients[i][j] / temp;
                }

                values[i] = values[i] / temp;

                if (i == k) {
                    continue;
                }

                for (std::size_t j = 0; j < size; ++j) {
                    coefficients[i][j] = coefficients[i][j] - coefficients[k][j];
                }

                values[i] = values[i] - values[k];
            }
        }


        for (std::size_t i = size ; i-- > 0;) {
            x[i] = values[i];

            for (std::size_t j = 0; j < i; ++j) {
                values[j] = values[j] - coefficients[j][i] * x[i];
            }
        }

        return x;
    }
}