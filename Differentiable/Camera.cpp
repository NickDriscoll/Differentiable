#include "head.h"

Vector2 Camera::getPosition()
{
	return position;
}

void Camera::update(Player player)
{
	position.x = player.getPosition().x - SCREEN_WIDTH / 2;
	position.y = player.getPosition().y - SCREEN_HEIGHT / 2;
}