#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_image/SDL_image.h>

static struct AppContext
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* labelTex = nullptr;
	SDL_GPUDevice* gpu = nullptr;
	SDL_FRect rect = {};
	SDL_AudioDeviceID audioDevice = 0;
	SDL_AppResult app_state = SDL_APP_CONTINUE;
} Context;

