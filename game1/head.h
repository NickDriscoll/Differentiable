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
const double TERMINAL_VELOCITY = 0.12;

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
	double x;
	double y;

	Vector2();
	Vector2(double x, double y);

	Vector2 add(Vector2 v);
	Vector2 multiply(double n);
};

//Axis Aligned Bounding Box
//Frequently used to represent static platforms.
class AABB
{
protected:
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
protected:
	SDL_Texture* texture;

	int textureWidth;
	int textureHeight;

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

	bool facingRight;
public:
	//Constructors
	MovingObject();
	MovingObject(char* pathToTexture, Vector2 Position, SDL_Renderer* r, bool FacingRight);

	//Getters
	SDL_Texture* getTexture();
	Vector2 getPosition();
	int getTextureWidth();
	int getTextureHeight();

	virtual void UpdatePhysics(double timeDelta);
};

class Player : public MovingObject
{
public:
	enum State
	{
		standing,
		walking,
		jumping
	};
protected:
	State currentState;
	State previousState;

public:
	Player();
	Player(char* path, Vector2 Position, SDL_Renderer* r, bool FacingRight);

	//Getters
	State getState();

	//Makes the player jump
	void jump();

	void accelerateLeft();
	void accelerateRight();
};