#include "heat_conductivity.hpp"
#include <stdexcept>

namespace Math {
    HeatConductivitySmoother::HeatConductivitySmoother(HeatConductivity k1, HeatConductivity k2, Real phaseTransitionTemperature, Real delta, SmoothOption option)
        : k1(k1), k2(k2), phaseTransitionTemperature(phaseTransitionTemperature), delta(delta), option(option) {
        approximate();
    }

    void HeatConductivitySmoother::approximate() {
        switch (option)
        {
        case LINEAR:
            approximateLinear();
            break;
        case POLYNOMIAL:
            approximatePolynomial();
            break;
        default:
            throw  std::logic_error("Invalid approximation option!");
        }
    }

    void HeatConductivitySmoother::approximateLinear() noexcept {
        auto k1Border = k1(phaseTransitionTemperature - delta);
        auto k2Border = k2(phaseTransitionTemperature + delta);

        kApproximated = [=](auto t) -> Real {
            return (t - phaseTransitionTemperature + delta) * (k2Border - k1Border) / 2 / delta + k1Border;
        };

    }

    void HeatConductivitySmoother::approximatePolynomial() noexcept {
        throw std::runtime_error("Not implemented!");
    }

    Real HeatConductivitySmoother::GetValue(Real temperature) const noexcept {
        if (temperature < phaseTransitionTemperature - delta) {
            return k1(temperature);
        } else if (temperature <= phaseTransitionTemperature + delta) {
            return kApproximated(temperature);
        } else {
            return k2(temperature);
        }
    }
}