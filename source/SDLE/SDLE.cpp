#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL_main.h>
#include "SDLE/SDLE.h"
#include "SDLE/UI/UI.h"
#include "SDLE/Assets.h"

#define FONT "JetBrainsMono-Regular.ttf"
#define LOGO "Logo.png"

#define GPU_FORMAT_VULKAN SDL_GPU_SHADERFORMAT_SPIRV
#define GPU_FORMAT_DIRECTX SDL_GPU_SHADERFORMAT_DXIL
#define GPU_FORMAT_METAL SDL_GPU_SHADERFORMAT_MSL

#ifdef _WIN32
	Uint32 WINDOW_FLAGS = SDL_WINDOW_VULKAN;
#elif defined(__APPLE__)
	Uint32 WINDOW_FLAGS = SDL_WINDOW_METAL;
#else
	Uint32 WINDOW_FLAGS = SDL_WINDOW_VULKAN;
#endif

using namespace std;

static SDL_AppResult SDL_Fail()
{
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error: %s", SDL_GetError());
    return SDL_APP_FAILURE;
}

static SDL_Texture* Message(std::string_view text, TTF_Font* font)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.data(), text.length(), { 255, 255, 255 });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Context.renderer, surface);

	SDL_DestroySurface(surface);

	return texture;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("SDLE", "1.0", "com.liamh.sdle");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
		return SDL_Fail();

    if (!TTF_Init())
		return SDL_Fail();

	if (!MIX_Init())
		return SDL_Fail();

	if (!SDL_CreateWindow("SDLE", 640, 480, SDL_WINDOW_VULKAN))
		return SDL_Fail();

	int drivers = SDL_GetNumVideoDrivers();

	for (int i = 0; i < drivers; i++)
	{
		SDL_Log("Video Driver %d: %s", i, SDL_GetVideoDriver(i));
	}

	Context.gpu = SDL_CreateGPUDevice(WINDOW_FLAGS, true, nullptr);

	if (!Context.gpu)
	{
		SDL_Log("Vulkan GPU could not be started, attempting DirectX");

		Context.gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXIL, true, nullptr);
		
		if (!Context.gpu)
			return SDL_Fail();
	}

	if (!SDL_ClaimWindowForGPUDevice(Context.gpu, Context.window))
		return SDL_Fail();

	if (!UI::Initialize())
		return SDL_Fail();

	TTF_Font* font = nullptr;

	if (!Assets::TryLoadFont(FONT, 16, &font))
		return SDL_Fail();

	Context.labelTex = Message("Hello, SDL!", font);

	auto texProps = SDL_GetTextureProperties(Context.labelTex);

	Context.rect =
	{
		.x = 0,
		.y = 0,
		.w = float(SDL_GetNumberProperty(texProps, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0)),
		.h = float(SDL_GetNumberProperty(texProps, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0))
	};
	
	SDL_SetRenderVSync(Context.renderer, -1);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
		Context.app_state = SDL_APP_SUCCESS;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    const double now = ((double)SDL_GetTicks()) / 1000.0;

	SDL_SetRenderDrawColorFloat(Context.renderer, 0.1, 0.1, 0.1, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(Context.renderer);

	SDL_RenderTexture(Context.renderer, Context.labelTex, NULL, &Context.rect);

    SDL_RenderPresent(Context.renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	UI::Cleanup();

	SDL_DestroyRenderer(Context.renderer);
	SDL_DestroyWindow(Context.window);

	TTF_Quit();
	MIX_Quit();
	SDL_Quit();
}
