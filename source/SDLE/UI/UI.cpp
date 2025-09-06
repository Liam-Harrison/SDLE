#include "SDLE/SDLE.h"
#include "SDLE/UI/UI.h"

TTF_TextEngine* UI::s_textEngine = nullptr;

bool UI::Initialize()
{
	if (s_textEngine == nullptr)
	{
		s_textEngine = TTF_CreateGPUTextEngine(Context.gpu);

		if (s_textEngine == nullptr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Failed to create text engine: %s", SDL_GetError());
			return false;
		}
	}

	return true;
}

void UI::Cleanup()
{
	if (s_textEngine)
	{
		TTF_DestroyGPUTextEngine(s_textEngine);
		s_textEngine = nullptr;
	}
}