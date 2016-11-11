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
	
	//TODO implement level files

	//Container that stores AABBs
	std::vector<AABB> aabbs;

	//Container that stores moving objects
	std::vector<MovingObject> movingObjects;

	//Font used
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\constan.ttf", FONT_SIZE);

	//Debug state flag
	bool debug = false;

	//Here we declare a flag to track if the program is still running
	bool running = true;

	//This is a variable to store the current event.
	SDL_Event e;
	
	//World camera
	Camera camera = Camera();

	//Test player
	Player player = Player("textures\\overman.png", Vector2(0, 0), renderer, true);

	//Level bounds
	AABB top = AABB(Vector2(0, -20), Vector2(1000, 10));
	aabbs.push_back(top);
	AABB bottom = AABB(Vector2(0, 500), Vector2(2000, 10));
	aabbs.push_back(bottom);

	//Platform
	AABB floor = AABB(Vector2(500, 350), Vector2(250, 20));
	AABB floor2 = AABB(Vector2(770, 200), Vector2(250, 20));
	aabbs.push_back(floor);
	aabbs.push_back(floor2);

	//Frametime vars
	Uint32 currentFrameTime = 0;
	Uint32 lastFrameTime = 0;

	//Game loop
	while (running)
	{
		//Process event queue until it is empty
		while (SDL_PollEvent(&e) != 0)
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
					debug = !debug;
					break;
				}
				/*
				case SDLK_l:
				{
					loadLevel("levels\\test.lvl", aabbs, movingObjects, player);
					break;
				}
				case SDLK_s:
				{
					saveLevel("levels\\test.lvl", aabbs, movingObjects);
					break;
				}
				*/
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
							player.setFacing(false);
							if (!player.isZiplining() && player.canJump())
							{
								player.accelerateLeft();
							}
						}
						//Right
						else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
						{
							player.setFacing(true);
							if (!player.isZiplining() && player.canJump())
							{
								player.accelerateRight();
							}
						}
						else
						{
							if (!player.isZiplining() && player.canJump())
							{
								player.stop();
							}
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

		//Stop player if he's on the ground and not moving


		//Debug printing
		//printf("Player position: %f, %f\n", player.getPosition().x, player.getPosition().y);

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

		//Update camera position
		camera.update(player);

		//Draw all AABBs, MovingObjects, and the player
		for (unsigned int i = 0; i < aabbs.size(); i++)
		{
			aabbs[i].draw(renderer, debug, camera);
		}
		//floor.draw(renderer, debug, camera);
		player.draw(renderer, debug, camera);

		//Update screen
		SDL_RenderPresent(renderer);

		//Set lastFrameTime equal to now
		lastFrameTime = currentFrameTime;
	}

	close(window, controller);

	return 0;
}