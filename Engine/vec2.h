#pragma once


class loc2 {
public:

	int x = 0;
	int y = 0;
	loc2() {}
	loc2(int X, int Y) :x(X), y(Y) {}

	bool operator==(const loc2& rhs)const {
		return (x == rhs.x) && (y == rhs.y);
	}
	bool operator!=(const loc2& rhs)const {
		return !(*this == rhs);
	}

	loc2& operator+=(const loc2& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	loc2& operator-=(const loc2& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	loc2 operator+(const loc2& rhs)const {
		return { x + rhs.x,y + rhs.y };
	}
	loc2 operator -(const loc2& rhs)const {
		return { x - rhs.x, y - rhs.y };
	}

};