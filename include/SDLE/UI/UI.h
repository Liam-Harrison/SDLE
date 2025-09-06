#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_ttf/SDL_textengine.h>

class UI
{
private:
	static TTF_TextEngine* s_textEngine;

public:
	static bool Initialize();
	static void Cleanup();

	static TTF_TextEngine* GetTextEngine() { return s_textEngine; }
};