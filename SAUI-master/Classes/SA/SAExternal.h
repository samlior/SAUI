#pragma once

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

typedef SAPoint SAOffsetPoint;
typedef SAPoint SASize;