#pragma once

#include <functional>

#include "math.hpp"
#include "../utils/utils.hpp"

namespace Math {

	using HeatCapacity = std::function<Real(Real)>;


	class HeatCapacitySmoother {
		HeatCapacity c1;
		HeatCapacity c2;

		HeatCapacity c1Approximated;
		HeatCapacity c2Approximated;

		Real phaseTransitionTemperature;
		Real delta;
		Real enthalpy;

		SmoothOption option;

	public:
		explicit HeatCapacitySmoother(HeatCapacity c1, HeatCapacity c2, Real phaseTransitionTemperature, Real enthalpy, Real delta, SmoothOption option);
		~HeatCapacitySmoother() = default;

		NODISCARD Real GetValue(Real temperature) const noexcept;

	private:
		void approximate();
		void approximateLinear();
		void approximateParabolic();

	};
    
}