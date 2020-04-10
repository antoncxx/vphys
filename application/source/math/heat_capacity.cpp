#include "heat_capacity.hpp"
#include <stdexcept>

#define INTEGRATION_STEPS_NUMBER 100

namespace Math {
    HeatCapacitySmoother::HeatCapacitySmoother(HeatCapacity c1, HeatCapacity c2, Real phaseTransitionTemperature, Real enthalpy, Real delta, SmoothOption option)
        : c1(c1), c2(c2), phaseTransitionTemperature(phaseTransitionTemperature), enthalpy(enthalpy), delta(delta), option(option) {
        approximate();
    }

    Real HeatCapacitySmoother::GetValue(Real temperature) const noexcept {
        if (temperature < phaseTransitionTemperature - delta) {
            return c1(temperature);
        }
        else if (temperature <= phaseTransitionTemperature) {
            return c1Approximated(temperature);
        }
        else if (temperature < phaseTransitionTemperature + delta) {
            return c2Approximated(temperature);
        }
        else {
            return c2(temperature);
        }
    }

    void HeatCapacitySmoother::approximate() {
        switch (option)
        {
        case LINEAR:
            approximateLinear();
            break;
        case PARABOLIC:
            approximateParabolic();
            break;
        case POLYNOMIAL:
            approximatePolynomial();
            break;
        default:
            throw std::logic_error("Invalid approximation option!");
        }
    }

    void HeatCapacitySmoother::approximateLinear() {
        auto c1Integrated = Integrate(c1, phaseTransitionTemperature - delta, phaseTransitionTemperature, delta / INTEGRATION_STEPS_NUMBER);
        auto c2Integrated = Integrate(c2, phaseTransitionTemperature, phaseTransitionTemperature + delta, delta / INTEGRATION_STEPS_NUMBER);

        auto c1Border = c1(phaseTransitionTemperature - delta);
        auto c2Border = c2(phaseTransitionTemperature + delta);

        auto c0 = 2 / delta * (enthalpy + c1Integrated + c2Integrated) - (c1Border + c2Border) / 2;

        c1Approximated = [=](auto t) -> Real {
            return (c0 - c1Border) * (t - phaseTransitionTemperature + delta) / delta + c1Border;
        };

        c2Approximated = [=](auto t) -> Real {
            return (c2Border - c0) * (t - phaseTransitionTemperature) / delta + c0;
        };
    }

    void HeatCapacitySmoother::approximateParabolic() {
        auto c1Integrated = Integrate(c1, phaseTransitionTemperature - delta, phaseTransitionTemperature, delta / INTEGRATION_STEPS_NUMBER);
        auto c2Integrated = Integrate(c2, phaseTransitionTemperature, phaseTransitionTemperature + delta, delta / INTEGRATION_STEPS_NUMBER);

        const auto pow2 = [](auto x) -> Real { return x * x; };
        const auto pow3 = [](auto x) -> Real { return x * x * x; };

        DataMatrix coefficients =
        {
            {                                         pow2(phaseTransitionTemperature - delta),                                                phaseTransitionTemperature - delta,     1,                                                                                  0,                                                                                  0,     0},
            {                                                                                0,                                                                                 0,     0,                                           pow2(phaseTransitionTemperature + delta),                                                 phaseTransitionTemperature + delta,     1},
            {                                                 pow2(phaseTransitionTemperature),                                                        phaseTransitionTemperature,     1,                                                  -pow2(phaseTransitionTemperature),                                                        -phaseTransitionTemperature,    -1},
            {                                                   2 * phaseTransitionTemperature,                                                                                 1,     0,                                                                                  0,                                                                                  0,     0},
            {                                                                                0,                                                                                 0,     0,                                               2 * pow2(phaseTransitionTemperature),                                                                                  1,     0},
            {(pow3(phaseTransitionTemperature) - pow3(phaseTransitionTemperature - delta)) / 3, (pow2(phaseTransitionTemperature) - pow2(phaseTransitionTemperature - delta)) / 2, delta, (-pow3(phaseTransitionTemperature) + pow3(phaseTransitionTemperature + delta)) / 3, (-pow2(phaseTransitionTemperature) + pow2(phaseTransitionTemperature + delta)) / 2, delta},
        };

        DataVector values =
        {
            c1(phaseTransitionTemperature - delta),
            c2(phaseTransitionTemperature + delta),
            0,
            0,
            0,
            enthalpy + (c1Integrated + c2Integrated),
        };

        auto x = SolveEquasions(coefficients, values, 6);

        Real a1 = x[0];
        Real b1 = x[1];
        Real d1 = x[2];

        c1Approximated = [=](auto t) -> Real {
            return a1 * pow2(t) + b1 * t + d1;
        };

        Real a2 = x[3];
        Real b2 = x[4];
        Real d2 = x[5];

        c2Approximated = [=](auto t) -> Real {
            return a2 * pow2(t) + b2 * t + d2;
        };
    }

    void HeatCapacitySmoother::approximatePolynomial() {
        throw std::runtime_error("Not implemented!");
    }

}