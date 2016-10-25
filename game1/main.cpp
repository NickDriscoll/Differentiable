#include "head.h"

int main(int argc, char* args[])
{
	// The window we shall be using
	SDL_Window* window = NULL;

	//The renderer for the window
	SDL_Renderer* renderer = NULL;
	
	//Container that stores AABBs
	std::vector<AABB> aabbs;

	//Container that stores moving objects
	std::vector<MovingObject> movingObjects;

	//Initialize SDL
	if (!init(window, renderer))
	{
		printf("Initialization error: %s\n", SDL_GetError());
		return -1;
	}

	//Here we declare a flag to track if the program is still running
	bool running = true;

	//This is a variable to store the current event
	SDL_Event e;
	
	//Test player
	Player guy = Player("overman.png", *(new Vector2(SCREEN_WIDTH / 2 - 16, SCREEN_HEIGHT / 2 - 32)), renderer, true);	

	//Game loop
	while (running)
	{
		//Process event queue until it is empty
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type != SDL_KEYDOWN)
			{
				switch (e.type)
				{
				case SDL_QUIT:
				{
					running = false;
					break;
				}
				}
			}
			else
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
				{
					running = false;
					break;
				}
				case SDLK_SPACE:
				{
					if (guy.getState() != guy.jumping)
					{
						guy.jump();
						printf("Boing\n");
						break;
					}
				}
				}
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		//Draw player character
		printf("round(x): %f\nround(y): %f", round(guy.getPosition().x), round(guy.getPosition().y));
		SDL_Rect playerRect = {(int)round(guy.getPosition().x) , (int)round(guy.getPosition().y), guy.getTextureWidth(), guy.getTextureHeight() };
		SDL_RenderCopy(renderer, guy.getTexture(), NULL, &playerRect);

		//Update physics
		guy.UpdatePhysics(1);

		//Update screen
		SDL_RenderPresent(renderer);
	}


	close(window);

	return 0;
}

bool init(SDL_Window* &window, SDL_Renderer* &renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return false;
	}

	//TODO: Learn how this code actually works.
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_Image could not initialize: %s\n", IMG_GetError());
		return false;
	}

	// Now that SDL has been initialized, we are going to create the window
	window = SDL_CreateWindow("Spirit", SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		return false;
	}

	// Get renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		return false;
	}

	return true;
}

SDL_Texture* loadTexture(char* path, SDL_Renderer* r)
{
	//Returned texture
	SDL_Texture* texture = NULL;

	//Load image to surface
	SDL_Surface* s = IMG_Load(path);
	if (s == NULL)
	{
		printf("Unable to load image: %s\n", IMG_GetError());
	}

	//Convert surface to texture
	texture = SDL_CreateTextureFromSurface(r, s);
	if (texture == NULL)
	{
		printf("Unable to convert to texture: %s\n", SDL_GetError());
	}
	SDL_FreeSurface(s);
	return texture;
}

void close(SDL_Window* &window)
{
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
}