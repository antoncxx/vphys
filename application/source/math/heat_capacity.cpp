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
        } else if (temperature <= phaseTransitionTemperature) {
            return c1Approximated(temperature);
        } else if (temperature < phaseTransitionTemperature + delta) {
            return c2Approximated(temperature);
        } else {
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
        default:
            throw  std::logic_error("Invalid approximation option!");
        }
    }

    void HeatCapacitySmoother::approximateLinear() noexcept {
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

    void HeatCapacitySmoother::approximateParabolic() noexcept {

    }

}