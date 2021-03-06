#include "head.h"

Menu::Menu()
{

}

Menu::Menu(const char* Title, char** Options, int NumberOfOptions)
{
	title = Title;
	options = Options;
	numberOfOptions = NumberOfOptions;
	currentOption = 0;
}

const char* Menu::getTitle()
{
	return title;
}

char** Menu::getOptions()
{
	return options;
}

void Menu::moveDown()
{
	currentOption++;
	if (currentOption >= numberOfOptions)
		currentOption = 0;
}

void Menu::moveUp()
{
	currentOption--;
	if (currentOption < 0)
		currentOption = numberOfOptions - 1;
}

std::string Menu::selectCurrentOption()
{
	return options[currentOption];
}

void Menu::draw(SDL_Renderer* r, TTF_Font* font)
{
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0xFF);
	SDL_RenderClear(r);

	//Draw menu title
	int titleLength = cstrLength(title);
	SDL_Texture* titleTexture = textureText(r, font, title);
	int titleDisplayWidth = titleLength * CHARACTER_WIDTH;
	SDL_Rect titleRect = { SCREEN_WIDTH / 2 - titleDisplayWidth / 2, CHARACTER_HEIGHT, titleDisplayWidth, 60};
	SDL_RenderCopy(r, titleTexture, NULL, &titleRect);
	SDL_DestroyTexture(titleTexture);

	//Draw menu options
	for (int i = 0; i < numberOfOptions; i++)
	{
		int optionLength = cstrLength(options[i]);
		SDL_Texture* optionTexture = textureText(r, font, options[i]);
		int optionDisplayWidth = optionLength * 20;
		SDL_Rect optionRect = {SCREEN_WIDTH / 2 - optionDisplayWidth / 2,  (SCREEN_HEIGHT / 2) + (60 * i), optionDisplayWidth, 30 };
		SDL_RenderCopy(r, optionTexture, NULL, &optionRect);
		SDL_DestroyTexture(optionTexture);

		//Draw arrows
		if (currentOption == i)
		{
			SDL_Texture* leftArrow = textureText(r, font, ">");
			SDL_Texture* rightArrow = textureText(r, font, "<");
			SDL_Rect leftRect = { SCREEN_WIDTH / 2 - optionDisplayWidth / 2 - 50, (SCREEN_HEIGHT / 2) + (60 * i), 20, 30};
			SDL_Rect rightRect = { SCREEN_WIDTH / 2 + optionDisplayWidth / 2 + 30, (SCREEN_HEIGHT / 2) + (60 * i), 20, 30 };
			SDL_RenderCopy(r, leftArrow, NULL, &leftRect);
			SDL_RenderCopy(r, rightArrow, NULL, &rightRect);
			SDL_DestroyTexture(leftArrow);
			SDL_DestroyTexture(rightArrow);
		}
	}
	SDL_RenderPresent(r);
}