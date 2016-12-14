#include "head.h"

Menu::Menu(const char* Title, char** Options, int NumberOfOptions)
{
	title = Title;
	options = Options;
	numberOfOptions = NumberOfOptions;
}

void Menu::moveDown()
{
	currentOption++;
	if (currentOption >= numberOfOptions)
		currentOption = numberOfOptions - 1;
}

void Menu::moveUp()
{
	currentOption--;
	if (currentOption < 0)
		currentOption = 0;
}

void Menu::draw(SDL_Renderer* r, TTF_Font* font)
{
	int titleLength = cstrLength(title);
	SDL_Texture* titleTexture = textureText(r, font, title);
	int titleDisplayWidth = titleLength * 40;
	SDL_Rect titleRect = { SCREEN_WIDTH / 2 - titleDisplayWidth / 2, 30, titleDisplayWidth, 60};
	SDL_RenderCopy(r, titleTexture, NULL, &titleRect);
	SDL_DestroyTexture(titleTexture);

	for (int i = 0; i < numberOfOptions; i++)
	{
		int optionLength = cstrLength(options[i]);
		SDL_Texture* optionTexture = textureText(r, font, options[i]);
		int optionDisplayWidth = optionLength * 20;
		SDL_Rect optionRect = {SCREEN_WIDTH / 2 - optionDisplayWidth / 2,  (SCREEN_HEIGHT / 2) + (60 * i), optionDisplayWidth, 30 };
		SDL_RenderCopy(r, optionTexture, NULL, &optionRect);
		SDL_DestroyTexture(optionTexture);
	}
}