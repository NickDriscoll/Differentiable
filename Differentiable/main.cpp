#include "head.h"

int main(int argc, char* args[])
{
	// The window we shall be using
	SDL_Window* window = NULL;

	//The renderer for the window
	SDL_Renderer* renderer = NULL;

	//Controller var
	SDL_Joystick* controller = NULL;

	//An array to hold all of the game menus
	Menu* menuArray = new Menu[MENUENUM_NR_ITEMS];

	//Initialize SDL
	if (!init(window, renderer, controller, menuArray))
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

	//Flag to track if editing
	bool inEditMode = false;

	//Flag to track if there was a joystick movement last frame
	bool joyEventLastFrame = false;
	bool joyEventThisFrame = false;

	//Tile index currently in use
	int currentlySelectedTileIndex = 0;

	//String holding what is currently in the console window
	std::string consoleString = "";

	//String holding editor text
	std::string editorString = "";

	//Tile sheet
	SDL_Texture *tileTexture = loadTexture("textures\\tile.png", renderer);
	
	//Create stack holding menus and push the main menu.
	std::stack<Menu> menus;
	menus.push(menuArray[MainMenu]);

	//This is a variable to store the current event.
	SDL_Event e;
	
	//World camera
	Camera camera = Camera();

	//Container that stores tiles
	std::vector<Tile> tiles;

	//Container that stores moving objects
	std::vector<MovingObject> movingObjects;

	//Player var
	Player player;

	//Load the level
	//loadLevel("levels\\test.lvl", tiles, movingObjects, player, renderer, separators);

	//Background image
	//SDL_Texture* background = loadTexture("", renderer);

	//Frametime vars
	Uint32 currentFrameTime = 0;
	Uint32 lastFrameTime = 0;

	//Game loop
	while (running)
	{
		//Frametiming
		currentFrameTime = SDL_GetTicks();

		//Render and handle any menus on the stack.
		if (menus.size() > 0)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_KEYDOWN)
				{
					eventKeyDownMenu(e, running, menus, tiles, movingObjects, player, renderer);
				}
				else if (e.type == SDL_JOYAXISMOTION)
				{
					eventJoystickMenu(e, menus, joyEventLastFrame);
				}
				else if (e.type == SDL_JOYBUTTONDOWN)
				{
					eventButtonMenu(e, running, menus, tiles, movingObjects, player, renderer);
				}
				else
				{
					eventMisc(e, running);
				}
			}

			//Draw menu, if statement necessary because it is possible
			//that by the end of this frame the menu stack is empty.
			if (menus.size() > 0)
				menus.top().draw(renderer, font);
		}
		//If there are no menus on the stack, run this
		else
		{
			//Process event queue until it is empty
			while (SDL_PollEvent(&e) != 0)
			{
				if (isConsoleUp)
				{
					eventIsConsoleUp(e, isConsoleUp, inEditMode, consoleString, editorString, tiles, movingObjects, player, renderer);
				}
				else if (inEditMode)
				{
					eventInEditMode(e, inEditMode, currentlySelectedTileIndex, tiles, camera, renderer);
				}
				else
				{
					if (e.type == SDL_KEYDOWN)
					{
						eventKeyDown(e, running, isConsoleUp, debug, consoleString, player, menus, menuArray[PauseMenu]);
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
						eventButton(e, running, debug, player, menus, menuArray[PauseMenu]);
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
			double timeDelta = (double)((currentFrameTime - lastFrameTime) / 1000.0);

			//Sometimes timeDelta is zero
			//No, I don't know why.

			//Update physics
			if (timeDelta > EPSILON && !isConsoleUp && !inEditMode)
			{
				player.UpdatePhysics(tiles, timeDelta);

				//Update camera position
				camera.update(player);
			}

			//Draw all tiles and the player
			for (unsigned int i = 0; i < tiles.size(); i++)
			{
				tiles[i].draw(renderer, debug, camera);
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

			//Edit related draw code here
			if (inEditMode)
			{
				//Draw text
				SDL_Texture* text = textureText(renderer, font, editorString.c_str());
				SDL_Rect rect = { 0, 0, (int)editorString.length() * 20 + 40, 30 };
				SDL_RenderCopy(renderer, text, NULL, &rect);
				SDL_DestroyTexture(text);

				//Draw tile next to words
				SDL_Rect tRects = { rect.w, 0, 32, 32 };
				SDL_Rect sRects = { currentlySelectedTileIndex * 32, 0, 32, 32 };
				SDL_RenderCopy(renderer, tileTexture, &sRects, &tRects);
			}

			//Debug print
			//printf("X:%f Y:%f\n", player.getPosition().x, player.getPosition().y);

			//Update screen
			SDL_RenderPresent(renderer);
		}

		//Set lastFrameTime equal to now
		lastFrameTime = currentFrameTime;
	}

	close(window, controller);

	return 0;
}