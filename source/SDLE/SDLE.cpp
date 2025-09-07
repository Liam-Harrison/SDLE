#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL_main.h>
#include "SDLE/SDLE.h"
#include "SDLE/UI/UI.h"
#include "SDLE/Assets.h"
#include "SDLE/UI/Label.h"
#include <vector>
#include <map>

#define APP_NAME "SDLE"

#define FONT "JetBrainsMono-Regular.ttf"
#define LOGO "Logo.png"

#if defined(__APPLE__)
	SDL_WindowFlags WINDOW_FLAGS = SDL_WINDOW_METAL;
	Uint32 GPU_FORMAT = SDL_GPU_SHADERFORMAT_SPIRV;
#else
	SDL_WindowFlags WINDOW_FLAGS = SDL_WINDOW_VULKAN;
	Uint32 GPU_FORMAT = SDL_GPU_SHADERFORMAT_SPIRV;
#endif

AppContext SDLE::Context {};

using namespace std;

static SDL_AppResult SDL_Fail()
{
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "%s", SDL_GetError());
    return SDL_APP_FAILURE;
}

static SDL_Texture* Message(std::string_view text, TTF_Font* font)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.data(), text.length(), { 255, 255, 255 });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(SDLE::Context.renderer, surface);

	SDL_DestroySurface(surface);

	return texture;
}

static Label label;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata(APP_NAME, "1.0", "com.liamh.sdle");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
		return SDL_Fail();

    if (!TTF_Init())
		return SDL_Fail();

	if (!MIX_Init())
		return SDL_Fail();

	SDLE::Context.window = SDL_CreateWindow(APP_NAME, 640, 480, WINDOW_FLAGS);

	if (!SDLE::Context.window)
		return SDL_Fail();

	SDLE::Context.renderer = SDL_CreateRenderer(SDLE::Context.window, NULL);

	if (!SDLE::Context.renderer)
		return SDL_Fail();

	if (!UI::Initialize())
		return SDL_Fail();

	TTF_Font* font = nullptr;

	if (!Assets::TryLoadFont(FONT, 16, &font))
		return SDL_Fail();

	label = Label(font, "Text Test");
	
	SDL_SetRenderVSync(SDLE::Context.renderer, -1);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
		SDLE::Context.app_state = SDL_APP_SUCCESS;

    return SDL_APP_CONTINUE;
}

static uint64_t lastNS = 0;
static double frameTimeSum = 0.0;
static int frameCount = 0;
static double avgFPS = 0.0;

SDL_AppResult SDL_AppIterate(void* appstate)
{
    const double now = ((double)SDL_GetTicks()) / 1000.0;
	const double delta = ((double)(SDL_GetTicksNS() - lastNS)) / 1000000.0;

	if (delta > 0.0)
	{
		frameTimeSum += delta / 1000.0; // ms to s
		frameCount++;

		if (frameTimeSum >= 0.25)
		{
			avgFPS = (double)frameCount / frameTimeSum;
			frameTimeSum = 0.0;
			frameCount = 0;
		}
	}
	
	SDL_SetRenderDrawColorFloat(SDLE::Context.renderer, 0.1, 0.1, 0.1, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(SDLE::Context.renderer);

	label.SetText("Time: {:.2f} FPS: {:.0f}", now, avgFPS);
	label.Render(0, 0);

    SDL_RenderPresent(SDLE::Context.renderer);

	lastNS = SDL_GetTicksNS();

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	UI::Cleanup();

	SDL_DestroyRenderer(SDLE::Context.renderer);
	SDL_DestroyWindow(SDLE::Context.window);

	TTF_Quit();
	MIX_Quit();
	SDL_Quit();
}
