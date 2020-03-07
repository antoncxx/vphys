#pragma once

#include <functional>

#include "../utils/utils.hpp"
#include "math.hpp"

namespace Math {

	using HeatCapacity = std::function<Real(Real)>;

	enum SmoothOption {
		LINEAR,
		PARABOLIC
	};

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
		HeatCapacitySmoother() = delete;

		Real GetValue(Real temperature) const noexcept;

	private:
		void approximate();
		void approximateLinear()    noexcept;
		void approximateParabolic() noexcept;

	};
    
}