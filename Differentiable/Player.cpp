#include "head.h"

Player::Player()
{
	isDead = false;
}

Player::Player(char* path, Vector2 Position, SDL_Renderer* r, bool FacingRight)
{
	texture = loadTexture(path, r);
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	position = Position;
	facingRight = FacingRight;
	boundingBox = AABB(Vector2(), Vector2(textureWidth, textureHeight));
	isDead = false;
}

bool Player::canJump()
{
	return onGround || wasOnGround;
}

bool Player::isZiplining()
{
	return ziplining;
}

bool Player::getIsDead()
{
	return isDead;
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

void Player::enterDoor(Door &currentDoor, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r)
{
	if (boundingBox.overlaps(currentDoor.getBoundingBox()))
	{
		printf("Now entering %s!\n", currentDoor.getConnectedRoom());
		loadLevel(currentDoor.getConnectedRoom(), tiles, movingObjects, player, currentDoor, r);
	}
}

void Player::die()
{
	isDead = true;

	//TODO: Add death animation.

}