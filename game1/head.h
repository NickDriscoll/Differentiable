#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_CENTER_X = 1920 / 2 - SCREEN_WIDTH / 2;
const int SCREEN_CENTER_Y = 1080 / 2 - SCREEN_HEIGHT / 2;
const int FONT_SIZE = 12;
const double EPSILON = 0.0001;
const double ACCELERATION_DUE_TO_GRAVITY = 500;
const double TERMINAL_VELOCITY = 400;


//Initializes SDL and any other components
bool init(SDL_Window* &window, SDL_Renderer* &renderer);


//Returns a pointer to an SDL_Texture.
//path: The filepath to the image (.png)
//r: the renderer
SDL_Texture* loadTexture(char* path, SDL_Renderer* r);


//Closes SDL components and frees memory
//window: Current window
void close(SDL_Window* &window);

//Loads text as texture
SDL_Texture* textureText(SDL_Renderer* r, TTF_Font* font, const char* message);



//A representation of a two-dimensional vector using floats
class Vector2
{
public:
	double x;
	double y;

	Vector2();
	Vector2(double x, double y);

	Vector2 operator+(const Vector2 &other);
	Vector2 operator*(const double &other);

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
	SDL_Texture* texture = NULL;

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
	void draw(SDL_Renderer* r, bool debug);
};



class MovingObject
{
protected:
	SDL_Texture* texture;

	int textureWidth;
	int textureHeight;

	Vector2 position;
	Vector2 lastPosition;
	
	Vector2 velocity = Vector2(0, 0);
	Vector2 lastVelocity = velocity;
	
	AABB boundingBox;
	AABB oldBoundingBox;

	//An offset for the center of the bounding box.
	Vector2 boundingBoxOffset;

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
	bool getFacing();

	//Setters
	void setFacing(bool facing);

	void UpdatePhysics(std::vector<AABB> boxes, double timeDelta);
	void draw(SDL_Renderer* r, bool debug);
	bool overlaps(AABB other);
};



class Player : public MovingObject
{
public:
	Player();
	Player(char* path, Vector2 Position, SDL_Renderer* r, bool FacingRight);

	//Getters
	bool isInAir();

	//Makes the player jump
	void jump();

	void accelerateLeft();
	void accelerateRight();
	void stop();
};