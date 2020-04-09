#include "math/stefan_problem_solver.h"

#include <iostream>
#include <chrono>
#include <thread>

int main() {
    using namespace Math;

    Real L1 = 1;
    Real L2 = 1;
    Real L3 = 1;

    const Real dx = 0.1;
    const Real dy = 0.1;
    const Real dz = 0.1;

    Grid grid(static_cast<std::size_t>(L1 / dx), static_cast<std::size_t>(L2 / dy), static_cast<std::size_t>(L3 / dz), dx, dy, dz);

    const Real phaseTrastionTemperature = 5;
    const Real enthalpy = 10;
    const Real delta = 1e-1;

    auto c1 = [](auto t) -> auto { return 2; };
    auto c2 = [](auto t) -> auto { return 20; };

    HeatCapacitySmoother capacity(c1, c2, phaseTrastionTemperature, enthalpy, delta, LINEAR); // smoothed heat capacity

    auto k1 = [](auto t) -> auto { return 0.2; };
    auto k2 = [](auto t) -> auto { return sin(6 * t) + 5; };

    HeatConductivitySmoother conductivity(k1, k2, phaseTrastionTemperature, delta, LINEAR); // smoothed heat conductivity 


    FirstKindBoundaryCondition bound([](auto time) { return 0; }, 
                                     [](auto time) { return 0; }, 
                                     [](auto time) { return 0; }, 
                                     [](auto time) { return 0; }, 
                                     [](auto time) { return 0; }, 
                                     [](auto time) { return 0; });

    InitialCondition initial([](auto x, auto y, auto z) { return 10; });

    StefanProblemSolver solver(grid, capacity, conductivity, bound, initial);

    auto start = std::chrono::high_resolution_clock::now();

    auto x = solver.Solve(1, 0.1);
    //auto x = solver.SolveParallel(1, 0.1, std::thread::hardware_concurrency());

   auto end = std::chrono::high_resolution_clock::now();
   std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

   //some output
   //for (auto row : x.back()[5]) {
   //    for (auto val : row) {
   //        std::cout << int(val) << " ";
   //    }
   //    std::cout << std::endl;
   //}
   system("pause");
    return 0;
}