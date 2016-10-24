#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_CENTER_X = 1920 / 2 - SCREEN_WIDTH / 2;
const int SCREEN_CENTER_Y = 1080 / 2 - SCREEN_HEIGHT / 2;

//Initializes SDL and any other components
bool init(SDL_Window* &window, SDL_Renderer* &renderer);

SDL_Texture* loadTexture(char* path, SDL_Renderer* &r);

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
	AABB(Vector2 Center, Vector2 HalfSize);
	bool overlaps(AABB other);
};