#include "stefan_problem_solver.h"
#include "../utils/thread_pool.hpp"

#define MAX_ITERATION 10

namespace Math {

    StefanProblemSolver::StefanProblemSolver(Math::Grid                       grid,
        Math::HeatCapacitySmoother       capacity,
        Math::HeatConductivitySmoother   conductivity,
        Math::FirstKindBoundaryCondition boundaryCondition,
        Math::InitialCondition           initialCondition)
        : grid(grid)
        , capacity(capacity)
        , conductivity(conductivity)
        , boundaryCondition(boundaryCondition)
        , initialCondition(initialCondition)
    {}

    std::vector<Math::DataCube> StefanProblemSolver::Solve(const Math::Real calculationTime, const Math::Real dt) {
        using namespace Math;

        std::vector<DataCube> data;
        data.emplace_back(std::move(GetInitialLayer()));


        Real time = 0;
        while (time < calculationTime) {

            DataCube prevIteration = data.back();
            std::size_t iteration = 0;

            do {
                DataCube nextIteration = MakeDataCube(grid.GetXNodesNumber() + 1, grid.GetYNodesNumber() + 1, grid.GetZNodesNumber() + 1);

                for (std::size_t j = 0; j < grid.GetYNodesNumber() + 1; ++j) {
                    for (std::size_t k = 0; k < grid.GetZNodesNumber() + 1; ++k) {
                        SolveX(nextIteration, prevIteration, data.back(), j, k, dt, time);
                    }
                }

                for (std::size_t i = 0; i < grid.GetXNodesNumber() + 1; ++i) {
                    for (std::size_t k = 0; k < grid.GetZNodesNumber() + 1; ++k) {
                        SolveY(nextIteration, prevIteration, data.back(), i, k, dt, time);
                    }
                }

                for (std::size_t i = 0; i < grid.GetXNodesNumber() + 1; ++i) {
                    for (std::size_t j = 0; j < grid.GetYNodesNumber() + 1; ++j) {
                        SolveZ(nextIteration, prevIteration, data.back(), i, j, dt, time);
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

        return data;
    }


    std::vector<Math::DataCube> StefanProblemSolver::SolveParallel(const Math::Real calculationTime, const Math::Real dt, std::size_t threadNumber) {
        using namespace Math;

        std::vector<DataCube> data;
        data.emplace_back(std::move(GetInitialLayer()));

        ThreadPool pool(threadNumber);
        std::vector<std::future<void>> storage;

        const auto synchronize = [&]() {
            for (auto&& task : storage) {
                task.get();
            }

            storage.clear();
        };

        Real time = 0;
        while (time < calculationTime) {

            DataCube prevIteration = data.back();
            std::size_t iteration = 0;

            do {
                DataCube nextIteration = MakeDataCube(grid.GetXNodesNumber() + 1, grid.GetYNodesNumber() + 1, grid.GetZNodesNumber() + 1);

                for (std::size_t j = 0; j < grid.GetYNodesNumber() + 1; ++j) {
                    for (std::size_t k = 0; k < grid.GetZNodesNumber() + 1; ++k) {
                        storage.push_back(
                            pool.AddTask(&StefanProblemSolver::SolveX, this, std::ref(nextIteration), std::cref(prevIteration), std::cref(data.back()), j, k, dt, time)
                            );
                    }
                }

                synchronize();

                for (std::size_t i = 0; i < grid.GetXNodesNumber() + 1; ++i) {
                    for (std::size_t k = 0; k < grid.GetZNodesNumber() + 1; ++k) {
                        storage.push_back(
                            pool.AddTask(&StefanProblemSolver::SolveY, this, std::ref(nextIteration), std::cref(prevIteration), std::cref(data.back()), i, k, dt, time)
                            );
                    }
                }

                synchronize();


                for (std::size_t i = 0; i < grid.GetXNodesNumber() + 1; ++i) {
                    for (std::size_t j = 0; j < grid.GetYNodesNumber() + 1; ++j) {
                        storage.push_back(
                            pool.AddTask(&StefanProblemSolver::SolveZ, this, std::ref(nextIteration), std::cref(prevIteration), std::cref(data.back()), i, j, dt, time)
                            );
                    }
                }

                synchronize();


                bool shouldStop = ShouldStop(nextIteration, prevIteration);
                prevIteration = std::move(nextIteration);

                if (shouldStop) {
                    break;
                }

            } while (iteration++ < MAX_ITERATION);

            data.emplace_back(std::move(prevIteration));
            time += dt;
        }

        return data;
    }

    Math::DataCube StefanProblemSolver::GetInitialLayer() const {
        Math::DataCube initialLayer = Math::MakeDataCube(grid.GetXNodesNumber() + 1,
            grid.GetYNodesNumber() + 1,
            grid.GetZNodesNumber() + 1);

        for (std::size_t i = 0; i < grid.GetXNodesNumber() + 1; ++i) {
            for (std::size_t j = 0; j < grid.GetYNodesNumber() + 1; ++j) {
                for (std::size_t k = 0; k < grid.GetZNodesNumber() + 1; ++k) {
                    initialLayer[i][j][k] = initialCondition.GetValue(i * grid.GetDX(), j * grid.GetDY(), k * grid.GetDZ());
                }
            }
        }

        return initialLayer;
    }


    void StefanProblemSolver::SolveX(Math::DataCube& nextIteration, const Math::DataCube& prevIteration, const Math::DataCube& prevLayer, std::size_t j, std::size_t k, Math::Real dt, Math::Real time) {
        const auto OmegaPlus = [&](std::size_t index) -> Math::Real {
            return dt * conductivity.GetValue((prevIteration[index][j][k] + prevIteration[index + 1][j][k]) / 2) /
                (grid.GetDX() * grid.GetDX() * capacity.GetValue(prevIteration[index][j][k]));
        };

        const auto OmegaMinus = [&](std::size_t index) -> Math::Real {
            return dt * conductivity.GetValue((prevIteration[index][j][k] + prevIteration[index - 1][j][k]) / 2) /
                (grid.GetDX() * grid.GetDX() * capacity.GetValue(prevIteration[index][j][k]));
        };

        const auto A = [&](std::size_t index) -> Math::Real {
            return -OmegaMinus(index);
        };

        const auto B = [&](std::size_t index) -> Math::Real {
            return -OmegaPlus(index);
        };

        const auto C = [&](std::size_t index) -> Math::Real {
            return 1 + OmegaPlus(index) + OmegaMinus(index);
        };

        const auto F = [&](std::size_t index) -> Math::Real {
            return prevLayer[index][j][k];
        };

        auto alpha = Math::MakeDataVector(grid.GetXNodesNumber() + 1);
        auto betta = Math::MakeDataVector(grid.GetXNodesNumber() + 1);

        alpha[1] = 0;
        betta[1] = boundaryCondition.GetXF1Value(time);

        for (std::size_t i = 2; i < grid.GetXNodesNumber() + 1; ++i) {
            alpha[i] = -B(i - 1) / (A(i - 1) * alpha[i - 1] + C(i - 1));
            betta[i] = (F(i - 1) - A(i - 1) * betta[i - 1]) / (A(i - 1) * alpha[i - 1] + C(i - 1));
        }

        nextIteration[grid.GetXNodesNumber()][j][k] = boundaryCondition.GetXF2Value(time);

        for (std::size_t i = grid.GetXNodesNumber(); i-- > 0;) {
            nextIteration[i][j][k] = alpha[i + 1] * nextIteration[i + 1][j][k] + betta[i + 1];
        }

    }

    void StefanProblemSolver::SolveY(Math::DataCube& nextIteration, const Math::DataCube& prevIteration, const Math::DataCube& prevLayer, std::size_t i, std::size_t k, Math::Real dt, Math::Real time) {
        const auto OmegaPlus = [&](std::size_t index) -> Math::Real {
            return dt * conductivity.GetValue((prevIteration[i][index][k] + prevIteration[i][index + 1][k]) / 2) /
                (grid.GetDY() * grid.GetDY() * capacity.GetValue(prevIteration[i][index][k]));
        };

        const auto OmegaMinus = [&](std::size_t index) -> Math::Real {
            return dt * conductivity.GetValue((prevIteration[i][index][k] + prevIteration[i][index - 1][k]) / 2) /
                (grid.GetDY() * grid.GetDY() * capacity.GetValue(prevIteration[i][index][k]));
        };

        const auto A = [&](std::size_t index) -> Math::Real {
            return -OmegaMinus(index);
        };

        const auto B = [&](std::size_t index) -> Math::Real {
            return -OmegaPlus(index);
        };

        const auto C = [&](std::size_t index) -> Math::Real {
            return 1 + OmegaPlus(index) + OmegaMinus(index);
        };

        const auto F = [&](std::size_t index) -> Math::Real {
            return nextIteration[i][index][k];
        };

        auto alpha = Math::MakeDataVector(grid.GetYNodesNumber() + 1);
        auto betta = Math::MakeDataVector(grid.GetYNodesNumber() + 1);

        alpha[1] = 0;
        betta[1] = boundaryCondition.GetYF1Value(time);

        for (std::size_t j = 2; j < grid.GetYNodesNumber() + 1; ++j) {
            alpha[j] = -B(j - 1) / (A(j - 1) * alpha[j - 1] + C(j - 1));
            betta[j] = (F(j - 1) - A(j - 1) * betta[j - 1]) / (A(j - 1) * alpha[j - 1] + C(j - 1));
        }

        nextIteration[i][grid.GetYNodesNumber()][k] = boundaryCondition.GetYF2Value(time);

        for (std::size_t j = grid.GetYNodesNumber(); j-- > 0;) {
            nextIteration[i][j][k] = alpha[j + 1] * nextIteration[i][j + 1][k] + betta[j + 1];
        }
    }

    void StefanProblemSolver::SolveZ(Math::DataCube& nextIteration, const Math::DataCube& prevIteration, const Math::DataCube& prevLayer, std::size_t i, std::size_t j, Math::Real dt, Math::Real time) {
        const auto OmegaPlus = [&](std::size_t index) -> Math::Real {
            return dt * conductivity.GetValue((prevIteration[i][j][index] + prevIteration[i][j][index + 1]) / 2) /
                (grid.GetDZ() * grid.GetDZ() * capacity.GetValue(prevIteration[i][j][index]));
        };

        const auto OmegaMinus = [&](std::size_t index) -> Math::Real {
            return dt * conductivity.GetValue((prevIteration[i][j][index] + prevIteration[i][j][index - 1]) / 2) /
                (grid.GetDZ() * grid.GetDZ() * capacity.GetValue(prevIteration[i][j][index]));
        };

        const auto A = [&](std::size_t index) -> Math::Real {
            return -OmegaMinus(index);
        };

        const auto B = [&](std::size_t index) -> Math::Real {
            return -OmegaPlus(index);
        };

        const auto C = [&](std::size_t index) -> Math::Real {
            return 1 + OmegaPlus(index) + OmegaMinus(index);
        };

        const auto F = [&](std::size_t index) -> Math::Real {
            return nextIteration[i][j][index];
        };

        auto alpha = Math::MakeDataVector(grid.GetZNodesNumber() + 1);
        auto betta = Math::MakeDataVector(grid.GetZNodesNumber() + 1);

        alpha[1] = 0;
        betta[1] = boundaryCondition.GetZF1Value(time);

        for (std::size_t k = 2; k < grid.GetZNodesNumber() + 1; ++k) {
            alpha[k] = -B(k - 1) / (A(k - 1) * alpha[k - 1] + C(k - 1));
            betta[k] = (F(k - 1) - A(k - 1) * betta[k - 1]) / (A(k - 1) * alpha[k - 1] + C(k - 1));
        }

        nextIteration[i][j][grid.GetZNodesNumber()] = boundaryCondition.GetZF2Value(time);

        for (std::size_t k = grid.GetZNodesNumber(); k-- > 0;) {
            nextIteration[i][j][k] = alpha[k + 1] * nextIteration[i][j][k + 1] + betta[k + 1];
        }
    }

    bool StefanProblemSolver::ShouldStop(const Math::DataCube& first, const Math::DataCube& second, Math::Real epsilon) {
        Math::Real maxDifference = std::numeric_limits<Math::Real>::min();

        for (std::size_t i = 0; i < first.size(); ++i) {
            for (std::size_t j = 0; j < first[i].size(); ++j) {
                for (std::size_t k = 0; k < first[i][j].size(); ++k) {
                    auto diff = std::abs(first[i][j][k] - second[i][j][k]);

                    if (maxDifference < diff) {
                        maxDifference = diff;

                        if (maxDifference > epsilon) {
                            return false;
                        }
                    }
                }
            }
        }

        return maxDifference <= epsilon;
    }
}