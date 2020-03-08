#pragma once 

#include "math.hpp"
#include <functional>

namespace Math {

    using HeatConductivity = std::function<Real(Real)>;

    class HeatConductivitySmoother {
        HeatConductivity k1;
        HeatConductivity k2;

        HeatConductivity kApproximated;

        Real phaseTransitionTemperature;
        Real delta;

        SmoothOption option;

    public:
    public:
        explicit HeatConductivitySmoother(HeatConductivity k1, HeatConductivity k2, Real phaseTransitionTemperature, Real delta, SmoothOption option);
        ~HeatConductivitySmoother() = default;

        Real GetValue(Real temperature) const noexcept;

    private:
        void approximate();
        void approximateLinear()     noexcept;
        void approximatePolynomial() noexcept;
    };


}