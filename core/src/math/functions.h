#pragma once
#include <cmath>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#include <gcem.hpp>
#pragma clang diagnostic pop

namespace spruce {
	template <typename T>
	[[nodiscard]] constexpr T clamp(const T& value, T min, T max) noexcept;

	template <typename T>
	[[nodiscard]] constexpr T lerp(T a, T b, T alpha) noexcept;

	template <typename T>
	[[nodiscard]] constexpr T sqrt(T value) noexcept;

	template <typename T>
	[[nodiscard]] constexpr T sin(T value) noexcept;

	template <typename T>
	[[nodiscard]] constexpr T cos(T value) noexcept;

	template <typename T>
	[[nodiscard]] constexpr T tan(T value) noexcept;

	template <typename T>
	[[nodiscard]] constexpr T asin(T value) noexcept;

	template <typename T>
	[[nodiscard]] constexpr T acos(T value) noexcept;

	template <typename T>
	[[nodiscard]] constexpr T atan(T value) noexcept;

	template <typename T>
	[[nodiscard]] constexpr T atan2(T a, T b) noexcept;
}

#include <math/functionsImpl.h>