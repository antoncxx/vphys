#pragma once

#include <vector>
#include <functional>

namespace Math {

    using Real       = double; 
    using DataVector = std::vector<Real>;

    using DataMatrix = std::vector<DataVector>;
    using DataCube   = std::vector<DataMatrix>;

    DataVector MakeDataVector(std::size_t xSize, Real defaulValue = Real());
    DataMatrix MakeDataMatrix(std::size_t xSize, std::size_t ySize, Real defaulValue = Real());
    DataCube   MakeDataCube(std::size_t xSize, std::size_t ySize, std::size_t zSize, Real defaulValue = Real());


    enum SmoothOption {
        LINEAR     = 1 << 3,
        PARABOLIC  = 1 << 4,
        POLYNOMIAL = 1 << 5 //TODO
    };

    template<class F, class T>
    auto Integrate(F&& func, T a, T b, T step) {
        auto steps = static_cast<std::uint64_t>((b - a) / step);
        auto result = T(0);
        for (std::uint64_t i = 0; i < steps; ++i) {
            result += (func(a + i * step) + func(a + (i + 1) * step)) / 2 * step;
        }
        return result;
    }

    class Grid {
        const std::size_t xNodes;
        const std::size_t yNodes;
        const std::size_t zNodes;

        const Real dx;
        const Real dy;
        const Real dz;
    public:
        explicit constexpr Grid(std::size_t xNodes, std::size_t yNodes, std::size_t zNodes, Real dx, Real dy, Real dz) 
            : xNodes(xNodes), yNodes(yNodes), zNodes(zNodes), dx(dx), dy(dy), dz(dz) {}

        [[nodiscard]] constexpr std::size_t GetXNodesNumber() const noexcept { return xNodes; }
        [[nodiscard]] constexpr std::size_t GetYNodesNumber() const noexcept { return yNodes; }
        [[nodiscard]] constexpr std::size_t GetZNodesNumber() const noexcept { return zNodes; }

        [[nodiscard]] constexpr Real GetDX() const noexcept { return dx; }
        [[nodiscard]] constexpr Real GetDY() const noexcept { return dy; }
        [[nodiscard]] constexpr Real GetDZ() const noexcept { return dz; }
    };

    class FirstKindBoundaryCondition {

    public:
        using Function = std::function<Real(Real)>;

        explicit FirstKindBoundaryCondition(Function fx1, Function fx2, Function fy1, Function fy2, Function fz1, Function fz2)
            : fx1(fx1), fx2(fx2), fy1(fy1), fy2(fy2), fz1(fz1), fz2(fz2) {}

        [[nodiscard]] inline Real GetXF1Value(Real time) const noexcept { return fx1(time); }
        [[nodiscard]] inline Real GetXF2Value(Real time) const noexcept { return fx2(time); }

        [[nodiscard]] inline Real GetYF1Value(Real time) const noexcept { return fy1(time); }
        [[nodiscard]] inline Real GetYF2Value(Real time) const noexcept { return fy2(time); }

        [[nodiscard]] inline Real GetZF1Value(Real time) const noexcept { return fz1(time); }
        [[nodiscard]] inline Real GetZF2Value(Real time) const noexcept { return fz2(time); }

    private:
        const Function fx1, fx2, fy1, fy2, fz1, fz2;
    };

    class InitialCondition {
    public:
        using Function = std::function<Real(Real, Real, Real)>;
        explicit InitialCondition(Function func) : func(func) {}

        [[nodiscard]] inline Real GetValue(Real x, Real y, Real z) const noexcept { return func(x, y, z); }

    private:
        Function func;
    };

    

    DataVector SolveEquasions(DataMatrix& coefficients, DataVector& values, std::size_t size, Real epsilon = Real(1e-2));
}