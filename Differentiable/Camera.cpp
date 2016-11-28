#include "head.h"

Vector2 Camera::getPosition()
{
	return position;
}

void Camera::update(Vector2 vector)
{
	position.x = vector.x;
	position.y = vector.y;
}

void Camera::update(Player player)
{
	position.x = player.getPosition().x - SCREEN_WIDTH / 2;
	position.y = player.getPosition().y - SCREEN_HEIGHT / 2;
}