#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <filesystem>

class Assets 
{
public:
	Assets() = delete;

	inline static const std::filesystem::path ResourcesPath = "resources";
	inline static const std::filesystem::path FontsPath = ResourcesPath / "fonts";
	inline static const std::filesystem::path TexturesPath = ResourcesPath / "textures";
	inline static const std::filesystem::path AudioPath = ResourcesPath / "audio";

	static const std::filesystem::path RelativeToFullPath(const std::filesystem::path& path);

	static bool TryLoadFont(const char* path, int fontSize, TTF_Font** font);
	static bool TryLoadImage(const char* path, SDL_Surface** surface);
	static bool TryLoadAudio(const char* path, MIX_Audio** audio);
};