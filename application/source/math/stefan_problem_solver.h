#pragma once

#include "heat_capacity.hpp"
#include "heat_conductivity.hpp"

namespace Math {

    class StefanProblemSolver
    {
        const Math::Grid                       grid;
        const Math::HeatCapacitySmoother       capacity;
        const Math::HeatConductivitySmoother   conductivity;
        const Math::InitialCondition           initialCondition;
        const Math::FirstKindBoundaryCondition boundaryCondition;

    public:
        explicit StefanProblemSolver(Math::Grid grid, Math::HeatCapacitySmoother capacity, Math::HeatConductivitySmoother conductivity, Math::FirstKindBoundaryCondition boundaryCondition, Math::InitialCondition initialCondition);
        ~StefanProblemSolver() = default;

        std::vector<Math::DataCube> Solve(const Math::Real calculationTime, const Math::Real dt);
        std::vector<Math::DataCube> SolveParallel(const Math::Real calculationTime, const Math::Real dt, std::size_t threadNumber);

    private:
        Math::DataCube GetInitialLayer() const;

        void SolveX(Math::DataCube& nextIteration, const Math::DataCube& prevIteration, const Math::DataCube& prevLayer, std::size_t j, std::size_t k, Math::Real dt, Math::Real time);
        void SolveY(Math::DataCube& nextIteration, const Math::DataCube& prevIteration, const Math::DataCube& prevLayer, std::size_t i, std::size_t k, Math::Real dt, Math::Real time);
        void SolveZ(Math::DataCube& nextIteration, const Math::DataCube& prevIteration, const Math::DataCube& prevLayer, std::size_t i, std::size_t j, Math::Real dt, Math::Real time);

        [[nodiscard]] bool ShouldStop(const Math::DataCube& first, const Math::DataCube& second, Math::Real epsilon = 1e-3);
    };
}

