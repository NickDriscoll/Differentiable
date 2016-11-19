#include "head.h"

void loadLevel(char* path, std::vector<AABB> &aabbs, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r, char separators[])
{
	//Clear the incoming vectors, this implicitly un-loads the current level.
	aabbs.clear();
	movingObjects.clear();

	std::fstream fs;
	fs.open(path, std::ios::in);

	std::queue<std::string> tokens = tokenize(fs, separators);
	fs.close();

	parselevel(tokens, aabbs, movingObjects, player, r);
}

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
		char* path = new char[fullPath.length()];
		strcpy(path, fullPath.c_str());

		loadLevel(path, aabbs, movingObjects, player, r, separators);
	}
	else
	{
		printf("Unrecognized command: \"%s\"\n\n", tokens.front().c_str());
	}

	tokens.pop();
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
	if (tokens.front().compare("</AABB>") != 0)
	{
		printf("Expected \"</AABB>\" but was \"%s\"\n\n", tokens.front().c_str());
		exit(0);
	}
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
	if (tokens.front().compare("</MovingObject>") != 0)
	{
		printf("Expected \"</MovingObject>\" but was \"%s\"\n\n", tokens.front().c_str());
		exit(0);
	}
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
	if (tokens.front().compare("</Player>") != 0)
	{
		printf("Expected \"</Player>\" but was \"%s\"\n\n", tokens.front().c_str());
		exit(0);
	}
	tokens.pop();

	//Stupid bullshit
	char* pathToTextureChar = new char[pathtoTexture.length()];
	strcpy(pathToTextureChar, pathtoTexture.c_str());

	//Done
	player = Player(pathToTextureChar, position, r, facingRight);
}
