#pragma once
typedef struct Vec2D {
	float x;
	float y;

	Vec2D() { x = 0; y = 0; };
	Vec2D(float x, float y) {
		this->x = x; this->y = y;
	}

	Vec2D operator+=(Vec2D& vec) { return Vec2D(this->x + vec.x, this->y + vec.y); };
} Vec2D;

inline Vec2D operator-(Vec2D& vec1, Vec2D& vec2) {
	return Vec2D(vec1.x - vec2.x, vec1.y - vec2.y);
}

inline Vec2D operator+(Vec2D& vec1, Vec2D& vec2) {
	return Vec2D(vec1.x + vec2.x, vec1.y + vec2.y);
}

inline Vec2D operator*(Vec2D& vec, float factor) {
	return Vec2D(vec.x * factor, vec.y * factor);
}

inline Vec2D operator/(Vec2D& vec, float factor) {
	return Vec2D(vec.x / factor, vec.y / factor);
}


