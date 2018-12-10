#include <math/quaternion.h>

namespace spruce {
	quaternion::quaternion() : x(0), y(0), z(0), w(1) {
	}

	quaternion::quaternion(const quaternion& quaternion) : x(quaternion.x), y(quaternion.y), z(quaternion.z), w(quaternion.w) {
	}

	quaternion::quaternion(const float& x, const float& y, const float& z, const float& w) : x(x), y(y), z(z), w(w) {
	}

	quaternion::quaternion(const float& pitch, const float& yaw, const float& roll) {
		/*
		float ys = sin(yaw /2);
		float yc = cos(yaw / 2);
		float ps = sin(pitch / 2);
		float pc = cos(pitch / 2);
		float rs = sin(roll / 2);
		float rc = cos(roll / 2);
		*/
		x = cos(yaw/2) * sin(pitch/2) * cos(roll/2) + sin(yaw/2) * cos(pitch/2) * sin(roll/2);//yc * pc * rs - ys * ps * rc;
		y = sin(yaw/2) * cos(pitch/2) * cos(roll/2) - cos(yaw/2) * sin(pitch/2) * sin(roll/2);//yc * ps * rc + ys * pc * rs;
		z = cos(yaw/2) * cos(pitch/2) * sin(roll/2) - sin(yaw/2) * sin(pitch/2) * cos(roll/2);//ys * pc * rc - yc * ps * rs;
		w = cos(yaw/2) * cos(pitch/2) * cos(roll/2) + sin(yaw/2) * sin(pitch/2) * sin(roll/2);//yc * pc * rc + ys * ps * rs;
		nor();
	}

	quaternion::quaternion(const vec3f& a, const vec3f& b) {
		vec3f crs = vec3f(a).crs(b);
		this->x = crs.x;
		this->y = crs.y;
		this->z = crs.z;
		this->w = (a.mag() * b.mag()) + a.dot(b);
		nor();
	}

	quaternion::quaternion(const vec3f& axis, const float& angle) {
		float sinHalfAngle = sin(angle / 2);
		this->x = axis.x * sinHalfAngle;
		this->y = axis.y * sinHalfAngle;
		this->z = axis.z * sinHalfAngle;
		this->w = cos(angle / 2);
		nor();
	}

	quaternion::~quaternion() {
	}

	quaternion& quaternion::set(const quaternion& quaternion) {
		this->x = quaternion.x;
		this->y = quaternion.y;
		this->z = quaternion.z;
		this->w = quaternion.w;
		return *this;
	}

	quaternion& quaternion::set(const float x, const float y, const float z, const float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		return *this;
	}

	quaternion& quaternion::set(const vec3f& axis, const float& angle) {
		float sinHalfAngle = sin(angle / 2);
		this->x = axis.x * sinHalfAngle;
		this->y = axis.y * sinHalfAngle;
		this->z = axis.z * sinHalfAngle;
		this->w = cos(angle / 2);
		nor();
		return *this;
	}

	quaternion& quaternion::set(const vec3f& a, const vec3f& b) {
		vec3f crs = vec3f(a).crs(b);
		this->x = crs.x;
		this->y = crs.y;
		this->z = crs.z;
		this->w = (a.mag() * b.mag()) + a.dot(b);
		nor();
		return *this;
	}

	quaternion& quaternion::set(const float& yaw, const float& pitch, const float& roll) {
		/*
		float ys = sin(yaw /2);
		float yc = cos(yaw / 2);
		float ps = sin(pitch / 2);
		float pc = cos(pitch / 2);
		float rs = sin(roll / 2);
		float rc = cos(roll / 2);
		*/
		x = cos(yaw/2) * sin(pitch/2) * cos(roll/2) + sin(yaw/2) * cos(pitch/2) * sin(roll/2);//yc * pc * rs - ys * ps * rc;
		y = sin(yaw/2) * cos(pitch/2) * cos(roll/2) - cos(yaw/2) * sin(pitch/2) * sin(roll/2);//yc * ps * rc + ys * pc * rs;
		z = cos(yaw/2) * cos(pitch/2) * sin(roll/2) - sin(yaw/2) * sin(pitch/2) * cos(roll/2);//ys * pc * rc - yc * ps * rs;
		w = cos(yaw/2) * cos(pitch/2) * cos(roll/2) + sin(yaw/2) * sin(pitch/2) * sin(roll/2);//yc * pc * rc + ys * ps * rs;
		nor();
		return *this;
	}

	void quaternion::toEuler(float& yaw, float& pitch, float& roll) {
		float t = y * x + z * w;
		int8 pole = t > 0.499f ? 1 : (t < -0.499f ? -1 : 0);
		bool locked = !(pole == 0);
		yaw = !locked ? atan2(2.0 * (y * w + x * z), 1.0 - 2.0 * (y * y + x * x)) : 0;
		pitch = !locked ? asin(2.0 * (w * x - z * y)) : pole * M_PI * 0.5;
		roll = !locked ? atan2(2.0 * (w * z + y * x), 1.0 - 2.0 * (x * x + z * z)) : pole * 2.0 * atan2(y, w);
	}

	float quaternion::mag2() const {
		return x *x + y * y + z * z + w * w;
	}

	float quaternion::mag() const {
		return sqrt(mag2());
	}

	quaternion& quaternion::nor() {
		float mag2 = quaternion::mag2();
		if (mag2 != 0 && mag2 != 1) {
			float mag = sqrt(mag2);
			x /= mag;
			y /= mag;
			z /= mag;
			w /= mag;
		}
		return *this;
	}

	quaternion& quaternion::conjugate() {
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	float quaternion::dot(const quaternion& quaternion) const {
		return x * quaternion.x + y * quaternion.y + z * quaternion.z + w * quaternion.w;
	}

	quaternion operator+(const quaternion& left, const quaternion& right) {
		float x = left.x + right.x;
		float y = left.y + right.y;
		float z = left.z + right.z;
		float w = left.w + right.w;
		return quaternion(x, y, z, w);
	}

	quaternion operator-(const quaternion& left, const quaternion& right) {
		float x = left.x - right.x;
		float y = left.y - right.y;
		float z = left.z - right.z;
		float w = left.w - right.w;
		return quaternion(x, y, z, w);
	}

	quaternion operator*(const quaternion& left, const quaternion& right) {
		float x = left.w * right.x + left.x * right.w + left.y * right.z - left.z * right.y;
		float y = left.w * right.y + left.y * right.w + left.z * right.x - left.x * right.z;
		float z = left.w * right.z + left.z * right.w + left.x * right.y - left.y * right.x;
		float w = left.w * right.w - left.x * right.x - left.y * right.y - left.z * right.z;
		return quaternion(x, y, z, w);
	}

	vec3f operator*(const vec3f& vector, const quaternion& quaternion) {
		spruce::quaternion conjugate = spruce::quaternion(quaternion).conjugate();
		spruce::quaternion result = quaternion * spruce::quaternion(vector.x, vector.y, vector.z, 0) * conjugate;
		return vec3f(result.x, result.y, result.z);
	}

	vec3f operator*(const quaternion& quaternion, const vec3f& vector) {
		spruce::quaternion conjugate = spruce::quaternion(quaternion).conjugate();
		spruce::quaternion result = quaternion * spruce::quaternion(vector.x, vector.y, vector.z, 0) * conjugate;
		return vec3f(result.x, result.y, result.z);
	}

	quaternion& quaternion::operator+=(const quaternion& quaternion) {
		x += quaternion.x;
		y += quaternion.y;
		z += quaternion.z;
		w += quaternion.w;
		return *this;
	}

	quaternion& quaternion::operator-=(const quaternion& quaternion) {
		x -= quaternion.x;
		y -= quaternion.y;
		z -= quaternion.z;
		w -= quaternion.w;
		return *this;
	}

	quaternion& quaternion::operator*=(const quaternion& quaternion) {
		float x = this->w * quaternion.x + this->x * quaternion.w + this->y * quaternion.z - this->z * quaternion.y;
		float y = this->w * quaternion.y + this->y * quaternion.w + this->z * quaternion.x - this->x * quaternion.z;
		float z = this->w * quaternion.z + this->z * quaternion.w + this->x * quaternion.y - this->y * quaternion.x;
		float w = this->w * quaternion.w - this->x * quaternion.x - this->y * quaternion.y - this->z * quaternion.z;
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, const quaternion& quaternion) {
		stream << "quaternion(" << quaternion.x << ", " << quaternion.y << ", " << quaternion.z << ", " << quaternion.w << ")";
		return stream;
	}

	quaternion quaternion::lerp(const quaternion& a, const quaternion& b, float alpha) {
		return quaternion((1 - alpha) * a.x + alpha * b.x, (1 - alpha) * a.y + alpha * b.y, (1 - alpha) * a.z + alpha * b.z, (1 - alpha) * a.w + alpha * b.w);
	}
}