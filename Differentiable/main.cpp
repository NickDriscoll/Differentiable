#include "head.h"

int main(int argc, char* args[])
{
	// The window we shall be using
	SDL_Window* window = NULL;

	//The renderer for the window
	SDL_Renderer* renderer = NULL;

	//Controller var
	SDL_Joystick* controller = NULL;

	//Initialize SDL
	if (!init(window, renderer, controller))
	{
		printf("Initialization error: %s\n", SDL_GetError());
		return -1;
	}
	
	//Font used
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\constan.ttf", FONT_SIZE);

	//Debug state flag
	bool debug = false;

	//Here we declare a flag to track if the program is still running
	bool running = true;

	//Flag to track if console is open
	bool isConsoleUp = false;

	//String holding what is currently in the console window
	std::string consoleString = "";

	//String holding editor text
	std::string editorString = "";

	//This is a variable to store the current event.
	SDL_Event e;
	
	//World camera
	Camera camera = Camera();
	
	//Container that stores AABBs
	std::vector<AABB> aabbs;

	//Container that stores moving objects
	std::vector<MovingObject> movingObjects;

	//Player var
	Player player;

	//Load the level
	loadLevel("levels/test.lvl", aabbs, movingObjects, player, renderer, separators);

	//Background image
	//SDL_Texture* background = loadTexture("", renderer);

	//Frametime vars
	Uint32 currentFrameTime = 0;
	Uint32 lastFrameTime = 0;

	//Scaling var used for zooming
	double scale = 1;

	//Game loop
	while (running)
	{
		//Process event queue until it is empty
		while (SDL_PollEvent(&e) != 0)
		{
			if (isConsoleUp) 
			{
				eventIsConsoleUp(e, isConsoleUp, consoleString, aabbs, movingObjects, player, renderer);
			}
			else
			{
				if (e.type == SDL_KEYDOWN)
				{
					eventKeyDown(e, running, isConsoleUp, debug, consoleString, player);
				}
				else if (e.type == SDL_KEYUP)
				{
					eventKeyUp(e, player);
				}
				else if (e.type == SDL_JOYAXISMOTION)
				{
					eventJoystick(e, player);
				}
				else if (e.type == SDL_JOYBUTTONDOWN)
				{
					eventButton(e, running, debug, player);
				}
				else
				{
					eventMisc(e, running);
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
		if (timeDelta > EPSILON && !isConsoleUp)
		{
			player.UpdatePhysics(aabbs, timeDelta);
		}
		//Update camera position
		camera.update(player);

		//Draw all AABBs, MovingObjects, and the player
		for (unsigned int i = 0; i < aabbs.size(); i++)
		{
			aabbs[i].draw(renderer, debug, camera);
		}
		player.draw(renderer, debug, camera);
		
		//Console related draw code here
		if (isConsoleUp)
		{
			SDL_Texture* text = textureText(renderer, font, ("> " + consoleString).c_str());
			SDL_Rect rect = { 0, 0, (int)consoleString.length() * 20 + 40, 30 };
			SDL_RenderCopy(renderer, text, NULL, &rect);
			SDL_DestroyTexture(text);
		}

		//Update screen
		SDL_RenderPresent(renderer);

		//Set lastFrameTime equal to now
		lastFrameTime = currentFrameTime;
	}

	close(window, controller);

	return 0;
}