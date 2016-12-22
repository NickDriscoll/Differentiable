#include "head.h"

Player::Player()
{
}

Player::Player(char* path, Vector2 Position, SDL_Renderer* r, bool FacingRight)
{
	texture = loadTexture(path, r);
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	position = Position;
	facingRight = FacingRight;
	boundingBox = AABB(Vector2(), Vector2(textureWidth, textureHeight));
}

bool Player::canJump()
{
	return onGround || wasOnGround;
}

bool Player::isZiplining()
{
	return ziplining;
}

void Player::jump()
{
	velocity.y = -400;
}

void Player::accelerateLeft()
{
	velocity.x = -200;
}

void Player::accelerateRight()
{
	velocity.x = 200;
}

void Player::stop()
{
	velocity.x = 0;
}

void Player::zipline()
{
	ziplining = !ziplining;
}

void Player::enterDoor()
{

}