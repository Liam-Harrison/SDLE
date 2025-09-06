#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <string>

class Label 
{
private:
    TTF_Text* text;
    TTF_Font* font;
    std::string content;

public:
    Label(TTF_Font* font, const std::string& content);
    ~Label();

    Label(const Label& other);
    Label& operator=(const Label& other);

    Label(Label&& other) noexcept;
    Label& operator=(Label&& other) noexcept;

    // Methods
    void SetText(const std::string& content);
    void Render(float x, float y);

    // Getters
    const std::string& GetText() const { return content; }
};