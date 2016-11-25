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
				if (e.type == SDL_KEYDOWN)
				{
					switch (e.key.keysym.sym)
					{
						case SDLK_BACKQUOTE:
						{
							isConsoleUp = !isConsoleUp;
							break;
						}
						case SDLK_BACKSPACE:
						{
							consoleString = consoleString.substr(0, consoleString.length() - 1);
							break;
						}
						case SDLK_RETURN:
						{
							//Parse
							std::queue<std::string> tokens = tokenize(consoleString, separators);
							parseCommand(tokens, aabbs, movingObjects, player, renderer, separators);
							consoleString = "";
							isConsoleUp = false;
							break;
						}
						default:
						{
							consoleString += e.key.keysym.sym;
							break;
							
						}

					}
				}
			}
			else
			{

#pragma region KeyDownEvents

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
						if (player.canJump())
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
						player.accelerateRight();
						player.setFacing(true);
						break;
					}

#ifdef _DEBUG


					case SDLK_BACKQUOTE:
					{
						isConsoleUp = !isConsoleUp;
						consoleString = "";
						break;
					}

					case SDLK_d:
					{
						debug = !debug;
						break;
					}
#endif
					}
				}

#pragma endregion

#pragma region KeyUpEvents

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

#pragma endregion

#pragma region ControllerEvents
				//Joystick stuff
				else if (e.type == SDL_JOYAXISMOTION)
				{
					//Motion on controller 0
					if (e.jaxis.which == 0)
					{
						//X axis motion
						if (e.jaxis.axis == 0)
						{
							//Left
							if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
							{
								player.accelerateLeft();
								player.setFacing(false);
							}
							//Right
							else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
							{
								player.accelerateRight();
								player.setFacing(true);
							}
							else
							{
								player.stop();
							}
						}
					}
				}

				//Button handing
				else if (e.type == SDL_JOYBUTTONDOWN)
				{
					if (e.jbutton.button == XBOX_360_A)
					{
						if (player.canJump())
						{
							player.jump();
						}
					}

					if (e.jbutton.button == XBOX_360_START)
					{
						running = false;
					}

					if (e.jbutton.button == XBOX_360_RB)
					{
						player.zipline();
					}

#ifdef _DEBUG
					if (e.jbutton.button == XBOX_360_SELECT)
					{
						debug = !debug;
					}
#endif

				}

#pragma endregion

#pragma region MiscEvents

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

#pragma endregion

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