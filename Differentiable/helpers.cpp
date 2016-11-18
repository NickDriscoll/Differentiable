#include "head.h"

bool init(SDL_Window* &window, SDL_Renderer* &renderer, SDL_Joystick* &controller)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		return false;
	}

	//Joystick init code
	if (SDL_NumJoysticks() < 1)
	{
		printf("Warning: No joysticks connected!\n\n");
	}
	else
	{
		controller = SDL_JoystickOpen(0);
		if (controller == NULL)
		{
			printf("Error loading joystick: %s\n", SDL_GetError());
		}
	}

	//Initialize image lib
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_Image could not initialize: %s\n", IMG_GetError());
		return false;
	}

	//Initialize ttf lib
	if (TTF_Init() == -1)
	{
		printf("TTF initialization error: %s\n", TTF_GetError());
		return false;
	}
	
	//Get screen dimensions.
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0, &mode);
	int width = mode.w;
	int height = mode.h;

	// Now that SDL has been initialized, we are going to create the window
	window = SDL_CreateWindow("Differentiable", width / 2 - SCREEN_WIDTH / 2, height / 2 - SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

//TODO finish close method (make it clean up ALL objects ALL subsystems etc.)
void close(SDL_Window* &window, SDL_Joystick* &controller)
{
	//Close game controller
	SDL_JoystickClose(controller);
	controller = NULL;

	SDL_DestroyWindow(window);
	
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* textureText(SDL_Renderer* r, TTF_Font* font, const char * message)
{
	if (font == NULL)
	{
		printf("Error loading font: %s\n", SDL_GetError());
	}
	SDL_Surface* s = TTF_RenderText_Solid(font, message, { 0xFF, 0xFF, 0xFF });
	SDL_Texture* t = SDL_CreateTextureFromSurface(r, s);
	SDL_FreeSurface(s);
	return t;
}

SDL_Rect newRect(Vector2 origin, Vector2 size)
{
	SDL_Rect rect = { (int)round(origin.x) , (int)round(origin.y), size.x, size.y };
	return rect;
}

void loadLevel(char* path, std::vector<AABB> &aabbs, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r, char separators[])
{
	//Clear the incoming vectors, this implicitly un-loads the current level.
	aabbs.clear();
	movingObjects.clear();

	std::fstream fs;
	fs.open("levels/test.lvl", std::ios::in);

	std::queue<std::string> tokens = tokenize(fs, separators);
	fs.close();

	parselevel(tokens, aabbs, movingObjects, player, r);
}

#pragma region Parsing

bool contains(char a, char arr[])
{
	for (int i = 0; arr[i] != '\0'; i++)
	{
		if (a == arr[i])
			return true;
	}
	return false;
}

std::string getNextWord(std::string string, int &position, char separators[])
{
	int beginning = position;

	//Ensure the index is pointing to a non-separator
	while (contains(string[beginning], separators))
	{
		beginning++;
	}

	int end = beginning;

	//Increment end until we hit another separator or the end of the line
	while (string[end] != '\0' && !contains(string[end], separators))
	{
		end++;
	}

	position = end;
	return string.substr(beginning, end);

}

std::queue<std::string> tokenize(std::fstream &in, char separators[])
{
	std::queue<std::string> tokens;
	
	while (!in.eof())
	{
		int currentPosition = 0;
		std::string currentToken;
		in >> currentToken;

		if (currentToken.compare("!") == 0)
		{
			in >> currentToken;
			while (currentToken.compare("!") != 0)
			{
				in >> currentToken;
				if (in.eof())
				{
					printf("Expected matching \"!\"\n\n");
					exit(0);
				}
			}
			in >> currentToken;
		}
		tokens.push(currentToken);
	}

	return tokens;
}

std::queue<std::string> tokenize(std::string &in, char separators[])
{
	std::queue<std::string> tokens;
	int currentPosition = 0;

	while (currentPosition < in.length())
	{
		tokens.push(getNextWord(in, currentPosition, separators));
	}

	return tokens;

}


void parseCommand(std::queue<std::string> &tokens, std::vector<AABB> &aabbs, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer *r, char separators[])
{
	if (tokens.front().compare("load") == 0)
	{
		tokens.pop();

		//Stupid bullshit
		std::string fullPath = ("levels\\" + tokens.front()).c_str();
		char * path = new char[fullPath.length()];
		strcpy(path, fullPath.c_str());

		loadLevel(path, aabbs, movingObjects, player, r, separators);
	}
}


void parselevel(std::queue<std::string> &tokens, std::vector<AABB> &aabbs, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer *r)
{
	//Throw away <level>
	tokens.pop();

	while (tokens.front().compare("</level>") != 0)
	{
		if (tokens.front().compare("<AABB>") == 0)
		{
			parseAABB(tokens, aabbs);
		}
		else if (tokens.front().compare("<MovingObject>") == 0)
		{
			parseMovingObject(tokens, movingObjects, r);
		}
		else if (tokens.front().compare("<Player>") == 0)
		{
			parsePlayer(tokens, player, r);
		}
		else
		{
			printf("Error parsing .lvl file\n\nInvalid token: %s\n\n", tokens.front().c_str());
			exit(0);
		}
	}
}

void parseAABB(std::queue<std::string> &tokens, std::vector<AABB> &aabbs)
{
	//AABB to be added
	AABB aabb;
	Vector2 origin;
	Vector2 size;

	//Throw away <AABB>
	tokens.pop();

	//Get xpos
	origin.x = std::stod(tokens.front());
	tokens.pop();

	//Get ypos
	origin.y = std::stod(tokens.front());
	tokens.pop();

	//Get length
	size.x = std::stod(tokens.front());
	tokens.pop();

	//Get height
	size.y = std::stod(tokens.front());
	tokens.pop();

	//Throw away </AABB>
	tokens.pop();

	//Done
	aabb.setOrigin(origin);
	aabb.setSize(size);

	aabbs.push_back(aabb);
}

void parseMovingObject(std::queue<std::string> &tokens, std::vector<MovingObject> &movingObjects, SDL_Renderer* r)
{
	//Properties
	std::string pathtoTexture;
	Vector2 position;
	bool facingRight;


	//Throw away <MovingObject>
	tokens.pop();

	//Get position
	position.x = std::stod(tokens.front());
	tokens.pop();

	position.y = std::stod(tokens.front());
	tokens.pop();

	//Get texture path
	pathtoTexture = tokens.front();
	tokens.pop();

	//Get facingRight
	facingRight = (bool)std::stoi(tokens.front());
	tokens.pop();

	//Throw away </MovingObject>
	tokens.pop();

	//Stupid bullshit
	char* pathToTextureChar = new char[pathtoTexture.length()];
	strcpy(pathToTextureChar, pathtoTexture.c_str());


	//Done
	movingObjects.push_back(MovingObject(pathToTextureChar, position, r, facingRight));

}

void parsePlayer(std::queue<std::string> &tokens, Player &player, SDL_Renderer *r)
{
	//Properties
	std::string pathtoTexture;
	Vector2 position;
	bool facingRight;


	//Throw away <Player>
	tokens.pop();

	//Get position
	position.x = std::stod(tokens.front());
	tokens.pop();

	position.y = std::stod(tokens.front());
	tokens.pop();

	//Get texture path
	pathtoTexture = tokens.front();
	tokens.pop();

	//Get facingRight
	facingRight = (bool)std::stoi(tokens.front());
	tokens.pop();

	//Throw away </Player>
	tokens.pop();

	//Stupid bullshit
	char* pathToTextureChar = new char[pathtoTexture.length()];
	strcpy(pathToTextureChar, pathtoTexture.c_str());

	//Done
	player = Player(pathToTextureChar, position, r, facingRight);
}

#pragma endregion