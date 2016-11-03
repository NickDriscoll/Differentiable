#include "head.h"

int main(int argc, char* args[])
{
	// The window we shall be using
	SDL_Window* window = NULL;

	//The renderer for the window
	SDL_Renderer* renderer = NULL;

	//Initialize SDL
	if (!init(window, renderer))
	{
		printf("Initialization error: %s\n", SDL_GetError());
		return -1;
	}

	
	//TODO implement level files

	//Container that stores AABBs
	std::vector<AABB> aabbs;

	//Container that stores moving objects
	std::vector<MovingObject> movingObjects;

	//Font used
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\comicbd.ttf", FONT_SIZE);

	//Debug state flag
	bool debug = false;

	//Here we declare a flag to track if the program is still running
	bool running = true;

	//This is a variable to store the current event.
	SDL_Event e;
	
	//Test player
	Player player = Player("resources\\overman.png", Vector2(SCREEN_WIDTH / 2 - 16, SCREEN_HEIGHT / 2 - 32), renderer, true);

	//Test platform
	AABB floor = AABB(Vector2(SCREEN_WIDTH / 2 - SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3), Vector2(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 10));
	aabbs.push_back(floor);

	//Frametime vars
	Uint32 currentFrameTime = 0;
	Uint32 lastFrameTime = 0;

	//Game loop
	while (running)
	{
		//Process event queue until it is empty
		//TODO Add controller support
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_KEYDOWN)
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
					if (!player.isInAir())
					{
						player.jump();
					}
					break;
				}
				case SDLK_LEFT:
				{
					player.accelerateLeft();
					player.setFacing(false);
					break;
				}
				case SDLK_RIGHT:
				{
					if (!player.isPushingRightWall())
					{
						player.accelerateRight();
						player.setFacing(true);
					}
					break;
				}
#ifdef _DEBUG
				case SDLK_BACKQUOTE:
				{
					debug = !debug;
					break;
				}
#endif
				}
			}
			else if (e.type == SDL_KEYUP)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_LEFT:
				{
					player.stop();
					break;
				}
				case SDLK_RIGHT:
				{
					player.stop();
					break;
				}
				}
			}
			else
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
		}

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		//Update physics w/ timedelta
		currentFrameTime = SDL_GetTicks();
		double timeDelta = (double)((currentFrameTime - lastFrameTime) / 1000.0);
		
		//Sometimes timeDelta is zero
		//No, I don't know why.
		if (timeDelta > EPSILON)
			player.UpdatePhysics(aabbs, timeDelta);

		//Draw all AABBs, MovingObjects, and the player
		floor.draw(renderer, debug);
		player.draw(renderer, debug);

		//Update screen
		SDL_RenderPresent(renderer);

		//Set lastFrameTime equal to now
		lastFrameTime = currentFrameTime;
	}

	close(window);

	return 0;
}