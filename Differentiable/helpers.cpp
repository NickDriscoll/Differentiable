#include "head.h"

int cstrLength(const char* string)
{
	int i = 0;
	while (string[i] != '\0')
	{
		i++;
	}
	return i;
}

bool init(SDL_Window* &window, SDL_Renderer* &renderer, SDL_Joystick* &controller, Menu* &menuArray, SDL_Texture* &AButton)
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

	//Initialize game menus

	//Create main menu
	int numberOfMainOptions = 4;
	char** menuOptions = new char*[numberOfMainOptions];
	menuOptions[0] = "New Game";
	menuOptions[1] = "Load";
	menuOptions[2] = "Options";
	menuOptions[3] = "Exit";
	menuArray[MainMenu] = Menu("Differentiable", menuOptions, numberOfMainOptions);

	//Create pause menu
	int numberOfPauseOptions = 3;
	char** pauseOptions = new char*[numberOfPauseOptions];
	pauseOptions[0] = "Resume";
	pauseOptions[1] = "Options";
	pauseOptions[2] = "Exit";
	menuArray[PauseMenu] = Menu("Differentiable", pauseOptions, numberOfPauseOptions);
	
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

	//Initialize global textures
	AButton = loadTexture("textures\\AButton.png", renderer);

	return true;
}

SDL_Texture* loadTexture(const char* path, SDL_Renderer* r)
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

SDL_Texture* textureText(SDL_Renderer* r, TTF_Font* font, const char* message)
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

char* stringToCharPointer(std::string string)
{
	//Stupid bullshit
	char* cptr = new char[string.length()];
	strcpy(cptr, string.c_str());
	return cptr;
}