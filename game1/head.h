#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <vector>
#include <ctime>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_CENTER_X = 1920 / 2 - SCREEN_WIDTH / 2;
const int SCREEN_CENTER_Y = 1080 / 2 - SCREEN_HEIGHT / 2;
const float TERMINAL_VELOCITY = 0.4;

//Initializes SDL and any other components
bool init(SDL_Window* &window, SDL_Renderer* &renderer);

//Returns a pointer to an SDL_Texture.
//path: The filepath to the image (.png)
//r: the renderer
SDL_Texture* loadTexture(char* path, SDL_Renderer* r);

//Closes SDL components and frees memory
//window: Current window
void close(SDL_Window* &window);

//A representation of a two-dimensional vector using floats
class Vector2
{
public:
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);

	Vector2 add(Vector2 v);
	Vector2 multiply(float n);
};

//Axis Aligned Bounding Box
//Frequently used to represent static platforms.
class AABB
{
private:
	Vector2 center;
	Vector2 halfSize;

public:
	AABB();
	AABB(Vector2 Center, Vector2 HalfSize);

	//Getters
	Vector2 getCenter();
	Vector2 getHalfSize();

	//Setters
	void setCenter(Vector2 newCenter);
	void setHalfSize(Vector2 newHalfSize);

	bool overlaps(AABB other);
};

class MovingObject
{
private:
	SDL_Texture* texture;

	//TODO: query texture
	Vector2 textureSize;

	Vector2 position;
	Vector2 lastPosition;
	
	Vector2 velocity = *new Vector2(0, 0);
	Vector2 lastVelocity = velocity;
	
	AABB boundingBox;
	Vector2 aabbOffset;

	bool pushedRightWall = false;
	bool pushesRightWall = false;

	bool pushedLeftWall = false;
	bool pushesLeftWall = false;

	bool wasOnGround = false;
	bool onGround = false;

	bool wasAtCeiling = false;
	bool atCeiling = false;
public:
	//Constructors
	MovingObject(char* pathToTexture, Vector2 Position, SDL_Renderer* r);
	MovingObject(char* pathToTexture, Vector2 Position, AABB BoundingBox, SDL_Renderer* r);

	//Getters
	SDL_Texture* getTexture();
	Vector2 getPosition();

	void UpdatePhysics(double timeDelta);
};

class Player : MovingObject
{

};