#include "head.h"

void eventIsConsoleUp(SDL_Event e, bool &isConsoleUp, bool &inEditMode, std::string &consoleString, std::string &editorString, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, Door &currentDoor, SDL_Renderer* r)
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
			std::queue<std::string> tokens = tokenize(consoleString);
			parseCommand(tokens, tiles, movingObjects, player, currentDoor, r, inEditMode, editorString, separators);
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

void eventKeyDown(SDL_Event e, bool &running, bool &isConsoleUp, bool &debug, std::string &consoleString, Player &player, std::stack<Menu> &menus, Menu &menu)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_ESCAPE:
	{
		menus.push(menu);
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
	if (e.jaxis.which == 0 && e.jaxis.axis == 0)
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

void eventButton(SDL_Event e, bool &running, bool &debug, Player &player, std::stack<Menu> &menus, Menu &menu, Door &currentDoor, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, SDL_Renderer* r)
{
	if (e.jbutton.button == XBOX_360_A)
	{
		if (player.canJump())
		{
			player.jump();
		}
	}

	if (e.jbutton.button == XBOX_360_B)
	{
		player.enterDoor(currentDoor, tiles, movingObjects, player, r);
	}

	if (e.jbutton.button == XBOX_360_START)
	{
		menus.push(menu);
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

void eventInEditMode(SDL_Event e, bool &inEditMode, int &currentlySelectedTileIndex, std::vector<Tile> &tiles, Camera &camera, Door &currentDoor, SDL_Renderer* r)
{
	//Get mouse position
	int x, y;
	SDL_GetMouseState(&x, &y);
	x += camera.getPosition().x;
	y += camera.getPosition().y;

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
		case SDLK_f:
		{
			currentDoor = Door(Vector2(x / 32 * 32, y / 32 * 32), "path here", r);
			break;
		}
		}
	}

	//Mouse handling
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
		{
			bool overlappingSomething = false;
			for (int i = 0; i < tiles.size() && !overlappingSomething; i++)
			{
				overlappingSomething = tiles[i].overlaps(Vector2(x, y));
			}

			if (!overlappingSomething)
			{

				//Messy conditionals for handling negative mouse position
				if (x < 0)
				{
					x /= TILE_WIDTH;
					x--;
				}
				else
				{
					x /= TILE_WIDTH;
				}

				//And for y
				if (y < 0)
				{
					y /= TILE_WIDTH;
					y--;
				}
				else
				{
					y /= TILE_WIDTH;
				}

				tiles.push_back(Tile(currentlySelectedTileIndex, Vector2(x, y), r));
				break;
			}
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

void eventKeyDownMenu(SDL_Event e, bool &running, std::stack<Menu> &menus, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, Door &currentDoor, SDL_Renderer* r)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_ESCAPE:
	{
		if (menus.top().getOptions()[0] != "New Game")
			menus.pop();
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
		parseMenuSelection(menus, tiles, movingObjects, player, currentDoor, r, running);
		break;
	}
	}
}

void eventJoystickMenu(SDL_Event e, std::stack<Menu> &menus, bool &isOutsideDeadzone, bool &wasOutsideDeadzone)
{
	if (e.jaxis.which == 0 && e.jaxis.axis == 1 && !wasOutsideDeadzone)
	{
		if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
		{
			menus.top().moveDown();
		}
		else if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
		{
			menus.top().moveUp();
		}
	}
}

void eventButtonMenu(SDL_Event e, bool &running, std::stack<Menu> &menus, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, Door &currentDoor, SDL_Renderer* r)
{
	switch (e.jbutton.button)
	{
	case XBOX_360_A:
	{
		std::string selectedOption = menus.top().selectCurrentOption();

		if (selectedOption.compare("New Game") == 0)
		{
			parseMenuSelection(menus, tiles, movingObjects, player, currentDoor, r, running);
		}
		else if (selectedOption.compare("Exit") == 0)
		{
			running = false;
		}
		else if (selectedOption.compare("Resume") == 0)
		{
			menus.pop();
		}

		break;
	}
	case XBOX_360_START:
	{
		if (menus.top().getOptions()[0] != "New Game")
			menus.pop();
		else
			parseMenuSelection(menus, tiles, movingObjects, player, currentDoor, r, running);
		break;
	}
	}
}