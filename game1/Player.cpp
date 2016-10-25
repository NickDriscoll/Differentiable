#include "head.h"

Player::Player()
{
	currentState = standing;
	previousState = currentState;
}

Player::Player(char* path, Vector2 Position, SDL_Renderer* r, bool FacingRight)
{
	currentState = standing;
	previousState = currentState;
	texture = loadTexture(path, r);
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	position = Position;
	facingRight = FacingRight;
}

Player::State Player::getState()
{
	return currentState;
}

void Player::jump()
{
	velocity.y = -0.2;
	currentState = jumping;
}