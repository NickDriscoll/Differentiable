#include "head.h"

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(float X, float Y)
{
	x = X;
	y = Y;
}

Vector2 Vector2::add(Vector2 v)
{
	return *new Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::multiply(float n)
{
	return *new Vector2(x * n, y * n);
}