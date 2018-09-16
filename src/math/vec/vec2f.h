#pragma once
#include <types.h>
#include <math.h>
#include <iostream>

namespace spruce {
	struct vec2f {
		float x;
		float y;

		vec2f();
		vec2f(const vec2f& vector);
		vec2f(const float& scalar);
		vec2f(const float& x, const float& y);
		~vec2f();

		float mag2() const;
		float mag() const;
		vec2f& nor();
		float dst2(const vec2f& vector) const;
		float dst(const vec2f& vector) const;
		float dot(const vec2f& vector) const;

		vec2f& add(const vec2f& vector);
		vec2f& sub(const vec2f& vector);

		vec2f& add(const float& value);
		vec2f& sub(const float& value);
		vec2f& scl(const float& value);
		vec2f& div(const float& value);

		friend vec2f operator+(const vec2f left, const vec2f& right);
		friend vec2f operator-(const vec2f left, const vec2f& right);

		friend vec2f operator+(const vec2f left, float value);
		friend vec2f operator-(const vec2f left, float value);
		friend vec2f operator*(const vec2f left, float value);
		friend vec2f operator/(const vec2f left, float value);

		bool operator==(const vec2f& vector) const;
		bool operator!=(const vec2f& vector) const;

		vec2f& operator+=(const vec2f& vector);
		vec2f& operator-=(const vec2f& vector);

		vec2f& operator+=(const float& value);
		vec2f& operator-=(const float& value);
		vec2f& operator*=(const float& value);
		vec2f& operator/=(const float& value);

		friend std::ostream& operator<<(std::ostream& stream, const vec2f& vector);

		static vec2f lerp(const vec2f& a, const vec2f& b, float alpha);
	};
}

namespace std {
	template <>
	struct hash<spruce::vec2f> {
		size_t operator()(const spruce::vec2f& v) const {
			return std::hash<float>()(v.x) ^ (std::hash<float>()(v.y) << 1);
		}
	};
}