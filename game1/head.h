#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_CENTER_X = 1920 / 2 - SCREEN_WIDTH / 2;
const int SCREEN_CENTER_Y = 1080 / 2 - SCREEN_HEIGHT / 2;

bool init(SDL_Window* &window, SDL_Surface* &surface);

SDL_Surface* loadSurface(char* path, SDL_Surface* surface);

void close(SDL_Window* &window);
