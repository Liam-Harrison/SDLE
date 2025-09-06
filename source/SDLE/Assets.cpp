#include "SDLE/Assets.h"

const std::filesystem::path Assets::RelativeToFullPath(const std::filesystem::path& path)
{
	const std::filesystem::path base = SDL_GetBasePath();
	return (base / path);
}

bool Assets::TryLoadFont(const char* path, int fontSize, TTF_Font** font)
{
	auto full = RelativeToFullPath(Assets::FontsPath / path);
	*font = TTF_OpenFont(full.string().c_str(), fontSize);
	return *font != NULL;
}

bool Assets::TryLoadImage(const char* path, SDL_Surface** surface)
{
	auto full = RelativeToFullPath(Assets::TexturesPath / path);
	*surface = IMG_Load(full.string().c_str());
	return *surface != NULL;
}

bool Assets::TryLoadAudio(const char* path, MIX_Audio** audio)
{
	auto full = RelativeToFullPath(Assets::AudioPath / path);
	*audio = MIX_LoadAudio(NULL, full.string().c_str(), false);
	return *audio != NULL;
}

