#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <vector>
#include <time.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_CENTER_X = 1920 / 2 - SCREEN_WIDTH / 2;
const int SCREEN_CENTER_Y = 1080 / 2 - SCREEN_HEIGHT / 2;

//Initializes SDL and any other components
bool init(SDL_Window* &window, SDL_Renderer* &renderer);

SDL_Texture* loadTexture(char* path, SDL_Renderer* r);

void close(SDL_Window* &window);

class Vector2
{
public:
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);
};

//Axis Aligned Bounding Box
class AABB
{
private:
	Vector2 center;
	Vector2 halfSize;

public:
	AABB();
	AABB(Vector2 Center, Vector2 HalfSize);
	bool overlaps(AABB other);
};

class MovingObject
{
private:
	SDL_Texture* texture;

	Vector2 position;
	Vector2 lastPosition;
	
	Vector2 velocity;
	Vector2 lastVelocity;
	
	AABB boundingBox;
	Vector2 aabbOffset;

	bool pushedRightWall;
	bool pushesRightWall;

	bool pushedLeftWall;
	bool pushesLeftWall;

	bool wasOnGround;
	bool onGround;

	bool wasAtCeiling;
	bool atCeiling;
public:
	//Constructors
	MovingObject(char* pathToTexture, Vector2 Position, SDL_Renderer* r);
	MovingObject(char* pathToTexture, Vector2 Position, AABB BoundingBox, SDL_Renderer* r);

	//Getters
	SDL_Texture* getTexture();

	void UpdatePhysics();
};