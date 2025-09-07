#include "SDLE/SDLE.h"
#include "SDLE/UI/UI.h"

TTF_TextEngine* UI::s_textEngine = nullptr;

bool UI::Initialize()
{
	if (s_textEngine == nullptr)
	{
		s_textEngine = TTF_CreateRendererTextEngine(SDLE::Context.renderer);

		if (s_textEngine == nullptr)
			return false;
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
