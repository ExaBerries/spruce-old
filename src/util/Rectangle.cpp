#include <util/Rectangle.h>

namespace spruce {
	Rectangle::Rectangle(vec2f size) : size(size) {
	}

	Rectangle::~Rectangle() {
	}

	bool Rectangle::overlap(const Rectangle& a, const vec2f& apos, const Rectangle& b, const vec2f& bpos) {
		bool colX = apos.x - a.size.x / 2 < bpos.x + b.size.x / 2 && apos.x + a.size.x / 2 > bpos.x - b.size.x / 2;
		bool colY = apos.y - b.size.y / 2 < bpos.y + b.size.y / 2 && apos.y + b.size.y / 2 > bpos.y - b.size.y / 2;
		return colX && colY;
	}
}
