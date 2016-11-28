#include "head.h"

void eventIsConsoleUp(SDL_Event e, bool &isConsoleUp, bool &inEditMode, std::string &consoleString, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r)
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
			parseCommand(tokens, tiles, movingObjects, player, r, inEditMode, separators);
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

void eventKeyDown(SDL_Event e, bool &running, bool &isConsoleUp, bool &debug, std::string &consoleString, Player &player)
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

void eventInEditMode(SDL_Event e, bool &inEditMode, std::vector<Tile> &tiles, Camera &camera, SDL_Renderer* r)
{
	//Key press handling
	switch (e.key.keysym.sym)
	{
	case SDLK_BACKQUOTE:
	{
		inEditMode = !inEditMode;
		break;
	}
	case SDLK_w:
	{
		camera.update(Vector2(camera.getPosition().x, camera.getPosition().y - 10));
		break;
	}
	case SDLK_s:
	{
		camera.update(Vector2(camera.getPosition().x, camera.getPosition().y + 10));
		break;
	}
	case SDLK_a:
	{
		camera.update(Vector2(camera.getPosition().x - 10, camera.getPosition().y));
		break;
	}
	case SDLK_d:
	{
		camera.update(Vector2(camera.getPosition().x + 10, camera.getPosition().y));
		break;
	}
	}

	//Mouse handling
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		x += camera.getPosition().x;
		y += camera.getPosition().y;
		printf("x: %d\ny: %d\n\n", x, y);
		tiles.push_back(Tile(1, Vector2(x / 32, y / 32), r));
		break;
	}
	case SDL_BUTTON_RIGHT:
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		x += camera.getPosition().x;
		y += camera.getPosition().y;
		for (unsigned int i = 0; i < tiles.size(); i++)
		{
			if (tiles[i].overlaps(Vector2(x, y)))
			{
				tiles.erase(tiles.begin() + i);
			}
		}
		break;
	}
	}

}