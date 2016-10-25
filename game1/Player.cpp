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
}

bool Player::isInAir()
{
	return !onGround;
}

void Player::jump()
{
	velocity.y = -0.2;
}

void Player::accelerateLeft()
{
	velocity.x = -0.05;
}

void Player::accelerateRight()
{
	velocity.x = 0.05;
}

void Player::stop()
{
	velocity.x = 0;
}

void Player::draw(SDL_Renderer* r)
{
	SDL_Rect playerRect = { (int)round(position.x) , (int)round(position.y), this->getTextureWidth(), this->getTextureHeight() };
	if (facingRight)
		SDL_RenderCopyEx(r, texture, NULL, &playerRect, 0, NULL, SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(r, texture, NULL, &playerRect, 0, NULL, SDL_FLIP_HORIZONTAL);
}