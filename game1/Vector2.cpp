#include "head.h"

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(double X, double Y)
{
	x = X;
	y = Y;
}

Vector2 Vector2::operator+(const Vector2& other)
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator*(const double& other)
{
	return Vector2(x * other, y * other);
}

Vector2 Vector2::add(Vector2 v)
{
	return *new Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::multiply(double n)
{
	return *new Vector2(x * n, y * n);
}