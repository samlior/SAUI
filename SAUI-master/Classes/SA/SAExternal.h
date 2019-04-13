#pragma once
#include "cocos2d.h"

class SAPoint
{
public:
	SAPoint():x(0), y(0) {}
	SAPoint(int outerX, int outerY) :x(outerX), y(outerY) {}
	virtual ~SAPoint() {}
	int x, y;

	SAPoint& operator += (const SAPoint& point)
	{
		x += point.x;
		y += point.y;
		return *this;
	}

	SAPoint& operator -= (const SAPoint& point)
	{
		x -= point.x;
		y -= point.y;
		return *this;
	}

	SAPoint operator + (const SAPoint& point) const
	{
		return SAPoint(
			x + point.x,
			y + point.y
		);
	}

	SAPoint operator - (const SAPoint& point) const
	{
		return SAPoint(
			x - point.x,
			y - point.y
		);
	}

	bool operator > (const SAPoint& point) const
	{
		return x == point.x ? y > point.y : x > point.x;
	}

	bool operator < (const SAPoint& point) const
	{
		return x == point.x ? y < point.y : x < point.x;
	}

	bool operator == (const SAPoint& point) const
	{
		return (x == point.x && y == point.y);
	}

	bool operator != (const SAPoint& point) const
	{
		return (x != point.x || y != point.y);
	}
};

using SAOffsetPoint = SAPoint;
using SASize = SAPoint;

class SAPosition
{
public:
	SAPosition() :x(0), y(0) {}
	SAPosition(float f1, float f2) :x(f1), y(f2) {}
	SAPosition(const SAPosition& pos) :x(pos.x), y(pos.y) {}
	SAPosition(const cocos2d::Size& size) :x(size.width), y(size.height) {}
	SAPosition(const cocos2d::Vec2& vec2) :x(vec2.x), y(vec2.y) {}

	cocos2d::Size toSize() { return cocos2d::Size(x, y); }
	cocos2d::Vec2 toVec2() { return cocos2d::Vec2(x, y); }

	void set(const cocos2d::Size& size) { x = size.width; y = size.height; }
	void set(const cocos2d::Vec2& vec2) { x = vec2.x; y = vec2.y; }

	float x;
	float y;
};