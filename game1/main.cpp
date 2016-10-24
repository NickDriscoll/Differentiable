#include "head.h"

int main(int argc, char* args[])
{
	// The window we shall be using
	SDL_Window* window = NULL;

	// The surface inside of the window
	SDL_Surface* surface = NULL;

	//Container that stores the current textures being used.
	std::vector<SDL_Texture*> textures;

	//Initialize SDL
	if (!init(window, surface))
	{
		printf("Initialization error: %s\n", SDL_GetError());
		return -1;
	}
	
	//Current background color
	Uint32 backgroundColor = SDL_MapRGB(surface->format, 0x0, 171, 0xFF);

	//Here we declare a flag to track if the program is still running
	bool running = true;

	//This is a variable to store the current event
	SDL_Event e;

	//Game loop
	while (running)
	{
		//Process event queue until it is empty
		while (SDL_PollEvent(&e) != 0)
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

		//Draw sky
		SDL_FillRect(surface, NULL, backgroundColor);

		//Draw character

		//Update the surface
		SDL_UpdateWindowSurface(window);

	}


	close(window);

	return 0;
}

bool init(SDL_Window* &window, SDL_Surface* &surface)
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
	window = SDL_CreateWindow("Not Braid or anything lol", SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		return false;
	}

	// Get surface
	surface = SDL_GetWindowSurface(window);

	return true;
}

SDL_Surface* loadSurface(char* path, SDL_Surface* screen)
{
	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* rawSurface = IMG_Load(path);
	if (rawSurface == NULL)
	{
		printf("Error loading image: %s", SDL_GetError());
		return optimizedSurface;
	}

	optimizedSurface = SDL_ConvertSurface(rawSurface, screen->format, NULL);
	if (optimizedSurface == NULL)
	{
		printf("Error optimizing image %s", SDL_GetError());
		return optimizedSurface;
	}

	SDL_FreeSurface(rawSurface);
	return optimizedSurface;
}

void close(SDL_Window* &window)
{
	SDL_DestroyWindow(window);

	SDL_Quit();
}