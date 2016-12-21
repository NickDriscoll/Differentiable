#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int FONT_SIZE = 72;
const double EPSILON = 0.0001;
const double ACCELERATION_DUE_TO_GRAVITY = 400;
const double TERMINAL_VELOCITY = 800;

//Separators
const char separators[] = " \n\0";

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 24000;

//Tile size
const int TILE_WIDTH = 32;

//Number of tile indices
const int TILE_MAX_INDEX = 4;

//Talking class prototypes?!?
class AABB;
class Door;
class Vector2;
class MovingObject;
class Player;
class Camera;
class Tile;
class Menu;

//Enumeration for Xbox 360 button indices
enum Xbox360Button
{
	XBOX_360_A = 0,
	XBOX_360_B = 1,
	XBOX_360_X = 2,
	XBOX_360_Y = 3,
	XBOX_360_LB = 4,
	XBOX_360_RB = 5,
	XBOX_360_SELECT = 6,
	XBOX_360_START = 7,
	XBOX_360_LSTICK = 8,
	XBOX_360_RSTICK = 9
};

//Collision enum
enum CollisionSide
{
	Top,
	Bottom,
	Left,
	Right,
	COLLISIONSIDE_NR_ITEMS
};

//Menu enum
enum MenuEnum
{
	MainMenu,
	PauseMenu,
	MENUENUM_NR_ITEMS
};

//A representation of a two-dimensional vector using doubles
class Vector2
{
public:
	double x;
	double y;

	Vector2();
	Vector2(double x, double y);

	Vector2 operator+(const Vector2 &other);
	Vector2 operator-(const Vector2 &other);
	Vector2 operator*(const double &other);

	Vector2 add(Vector2 v);
	Vector2 multiply(double n);
};


//A representation for the game camera
class Camera
{
private:
	Vector2 position;

public:
	Vector2 getPosition();
	void update(Vector2 vector);
	void update(Player player);
};


class Tile
{
protected:
	SDL_Texture* texture;
	int textureIndex;
	Vector2 position;

public:
	Tile(int TextureIndex, Vector2 Position, SDL_Renderer* r);

	//Getters
	Vector2 getPosition();
	int getTextureIndex();

	//Settings
	void setPosition(Vector2 Position);
	
	//Returns AABB representation for historic reasons
	AABB aabb();

	bool overlaps(Vector2 point);
	void draw(SDL_Renderer* r, bool debug, Camera camera);
};

//Axis Aligned Bounding Box
//Now used exclusively as hitboxes
class AABB
{
protected:
	Vector2 origin;
	Vector2 size;
	SDL_Color color;

public:
	AABB();
	AABB(const Vector2 &Origin, const Vector2 &Size);
	AABB(const Vector2 &Origin, const Vector2 &Size, SDL_Color Color);

	//Getters
	Vector2 getOrigin();
	Vector2 getSize();

	//Setters
	void setOrigin(Vector2 newOrigin);
	void setSize(Vector2 newSize);

	bool overlaps(Vector2 position);
	bool overlaps(AABB other);
	bool* collisionSide(AABB other);

	void draw(SDL_Renderer* r, bool debug, Camera camera);
};


class Door : public AABB
{
protected:
	char* path;

public:
	Door(const Vector2 &Origin, char* Path);
};


class MovingObject
{
protected:
	SDL_Texture* texture;

	int textureWidth;
	int textureHeight;

	Vector2 position;
	Vector2 lastPosition;
	
	Vector2 velocity = Vector2(0, 0);
	Vector2 lastVelocity = velocity;
	
	AABB boundingBox;
	AABB oldBoundingBox;

	bool ziplining = false;

	bool pushedRightWall = false;
	bool pushesRightWall = false;

	bool pushedLeftWall = false;
	bool pushesLeftWall = false;

	bool wasOnGround = false;
	bool onGround = false;

	bool wasAtCeiling = false;
	bool atCeiling = false;

	bool facingRight;

public:
	//Constructors
	MovingObject();
	MovingObject(char* pathToTexture, Vector2 Position, SDL_Renderer* r, bool FacingRight);

	//Getters
	SDL_Texture* getTexture();
	Vector2 getPosition();
	Vector2 getVelocity();
	int getTextureWidth();
	int getTextureHeight();
	bool getFacing();

	//Setters
	void setTexture(char* texturePath, SDL_Renderer* r);
	void setFacing(bool facing);
	void setPosition(Vector2 newPos);

	void UpdatePhysics(std::vector<Tile> tiles, double timeDelta);
	void draw(SDL_Renderer* r, bool debug, Camera camera);
};



class Player : public MovingObject
{
public:
	Player();
	Player(char* path, Vector2 Position, SDL_Renderer* r, bool FacingRight);

	//Getters
	bool canJump();
	bool isZiplining();

	//Makes the player jump
	void jump();
	void zipline();

	void accelerateLeft();
	void accelerateRight();
	void stop();
};

//Class that represents a menu
class Menu
{
private:
	const char* title;
	char** options;
	int currentOption;
	int numberOfOptions;
public:

	//Constructor
	Menu();
	Menu(const char* Title, char** Options, int NumberOfOptions);

	//Getters
	const char* getTitle();
	char** getOptions();

	//Misc functions
	void moveDown();
	void moveUp();
	std::string selectCurrentOption();
	void draw(SDL_Renderer* r, TTF_Font* font);
};

//Gets length of cstr
int cstrLength(const char* string);

//Initializes SDL and any other components
bool init(SDL_Window* &window, SDL_Renderer* &renderer, SDL_Joystick* &controller, Menu* &menuArray);

//Returns a pointer to an SDL_Texture.
//path: The filepath to the image (.png)
//r: the renderer
SDL_Texture* loadTexture(const char* path, SDL_Renderer* r);

//Closes SDL components and frees memory
//window: Current window
//controller: Current controller if applicable.
void close(SDL_Window* &window, SDL_Joystick* &controller);

//Loads text as texture
//r: Current renderer
//font: font to use
//message: String to turn into message.
SDL_Texture* textureText(SDL_Renderer* r, TTF_Font* font, const char* message);

//"Helpful" wrapper for SDL_Rect struct initialization
SDL_Rect newRect(Vector2 origin, Vector2 size);

char* stringToCharPointer(std::string string);

#pragma region Parsing

bool contains(char a, const char arr[]);

std::string getNextWord(std::string string, int &position, const char separators[]);

std::queue<std::string> tokenize(std::fstream &in, const char separators[]);

std::queue<std::string> tokenize(std::string &in, const char separators[]);

void parseCommand(std::queue<std::string> &tokens, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer *r, bool &inEditMode, std::string &editorString, const char separators[]);

void parseMenuSelection(std::stack<Menu> &menus, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r, bool &running);

bool fileExists(char* path);

void loadLevel(char* path, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r, const char separators[]);

void saveLevel(char* path, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r);

#pragma endregion

#pragma region RecursiveDescent

void parselevel(std::queue<std::string> &tokens, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer *r);

void parseTile(std::queue<std::string> &tokens, std::vector<Tile> &tiles, SDL_Renderer* r);

void parseMovingObject(std::queue<std::string> &tokens, std::vector<MovingObject> &movingObjects, SDL_Renderer *r);

void parsePlayer(std::queue<std::string> &tokens, Player &player, SDL_Renderer *r);

#pragma endregion

#pragma region EventHandlers

void eventIsConsoleUp(SDL_Event e, bool &isConsoleUp, bool &inEditMode, std::string &consoleString, std::string &editorString, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r);

void eventKeyDown(SDL_Event e, bool &running, bool &isConsoleUp, bool &debug, std::string &consoleString, Player &player, std::stack<Menu> &menus, Menu &mainMenu);

void eventKeyUp(SDL_Event e, Player &player);

void eventJoystick(SDL_Event e, Player &player);

void eventButton(SDL_Event e, bool &running, bool &debug, Player &player, std::stack<Menu> &menus, Menu &menu);

void eventMisc(SDL_Event e, bool &running);

void eventInEditMode(SDL_Event e, bool &inEditMode, int &currentlySelectedTileIndex, std::vector<Tile> &tiles, Camera &camera, SDL_Renderer* r);

#pragma endregion

#pragma region MenuEventHandlers

void eventKeyDownMenu(SDL_Event e, bool &running, std::stack<Menu> &menus, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r);

void eventJoystickMenu(SDL_Event e, std::stack<Menu> &menus, bool joyEventLastFrame);

void eventButtonMenu(SDL_Event e, bool &running, std::stack<Menu> &menus, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r);

#pragma endregion