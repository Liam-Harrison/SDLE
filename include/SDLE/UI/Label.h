#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <string>
#include <format>

class Label 
{
private:
    TTF_Text* text;
    TTF_Font* font;
    std::string content;

public:
    Label();
    Label(TTF_Font* font, const std::string& content);
    ~Label();

    Label(const Label& other);
    Label& operator=(const Label& other);

    Label(Label&& other) noexcept;
    Label& operator=(Label&& other) noexcept;

    // Methods
    template<typename... Args>
    void SetText(const std::string& format, Args&&... args) { SetText(std::vformat(format, std::make_format_args(args...))); }
    void SetText(const std::string& content);
    void Render(float x, float y);

    // Getters
    const std::string& GetText() const { return content; }
};