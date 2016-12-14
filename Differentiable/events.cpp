#include "head.h"

void eventIsConsoleUp(SDL_Event e, bool &isConsoleUp, bool &inEditMode, std::string &consoleString, std::string &editorString, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r)
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
			parseCommand(tokens, tiles, movingObjects, player, r, inEditMode, editorString, separators);
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

void eventKeyDown(SDL_Event e, bool &running, bool &isConsoleUp, bool &debug, std::string &consoleString, Player &player, std::stack<Menu> &menus, Menu &mainMenu)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_ESCAPE:
	{
		menus.push(mainMenu);
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

#ifdef DEBUG_FEATURES
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

void eventKeyUp(SDL_Event e, Player &player)
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

void eventJoystick(SDL_Event e, Player &player)
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

void eventButton(SDL_Event e, bool &running, bool &debug, Player &player)
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

#ifdef DEBUG_FEATURES
	if (e.jbutton.button == XBOX_360_SELECT)
	{
		debug = !debug;
	}
#endif

}

void eventMisc(SDL_Event e, bool &running)
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

void eventInEditMode(SDL_Event e, bool &inEditMode, int &currentlySelectedTileIndex, std::vector<Tile> &tiles, Camera &camera, SDL_Renderer* r)
{
	//Key press handling
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_BACKQUOTE:
		{
			inEditMode = !inEditMode;
			break;
		}
		case SDLK_w:
		{
			camera.update(Vector2(camera.getPosition().x, camera.getPosition().y - TILE_WIDTH));
			break;
		}
		case SDLK_s:
		{
			camera.update(Vector2(camera.getPosition().x, camera.getPosition().y + TILE_WIDTH));
			break;
		}
		case SDLK_a:
		{
			camera.update(Vector2(camera.getPosition().x - TILE_WIDTH, camera.getPosition().y));
			break;
		}
		case SDLK_d:
		{
			camera.update(Vector2(camera.getPosition().x + TILE_WIDTH, camera.getPosition().y));
			break;
		}
		}
	}


	//Draw mouse dot
	int x, y;
	SDL_GetMouseState(&x, &y);
	x += camera.getPosition().x;
	y += camera.getPosition().y;

	//Mouse handling
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
		{
			//Messy conditionals for handling negative mouse position
			if (x < 0)
			{
				x /= 32;
				x--;
			}
			else
			{
				x /= 32;
			}

			//And for y
			if (y < 0)
			{
				y /= 32;
				y--;
			}
			else
			{
				y /= 32;
			}

			tiles.push_back(Tile(currentlySelectedTileIndex, Vector2(x, y), r));
			break;
		}
		case SDL_BUTTON_RIGHT:
		{
			for (int i = 0; i < tiles.size(); i++)
			{
				if (tiles[i].overlaps(Vector2(x, y)))
				{
					tiles.erase(tiles.begin() + i);
					break;
				}
			}
			break;
		}
		}
	}

	//Scroll wheel handling
	if (e.type == SDL_MOUSEWHEEL)
	{
		if (e.wheel.y < 0)
		{
			currentlySelectedTileIndex++;
		}
		else
		{
			currentlySelectedTileIndex--;
		}

		//Adjust it to be back in bounds
		if (currentlySelectedTileIndex < 0)
		{
			currentlySelectedTileIndex = 0;
		}
		else if (currentlySelectedTileIndex > TILE_MAX_INDEX)
		{
			currentlySelectedTileIndex = TILE_MAX_INDEX;
		}
	}
}

void eventKeyDownMenu(SDL_Event e, bool &running, std::stack<Menu> &menus, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_ESCAPE:
	{
		running = false;
		break;
	}
	case SDLK_DOWN:
	{
		menus.top().moveDown();
		break;
	}
	case SDLK_UP:
	{
		menus.top().moveUp();
		break;
	}
	case SDLK_RETURN:
	{
		std::string selectedOption = menus.top().selectCurrentOption();

		if (selectedOption.compare("Play") == 0)
		{
			loadLevel("levels\\test.lvl", tiles, movingObjects, player, r, separators);
			menus.pop();
		}
		else if (selectedOption.compare("Exit") == 0)
		{
			running = false;
		}

		break;
	}
	}
}