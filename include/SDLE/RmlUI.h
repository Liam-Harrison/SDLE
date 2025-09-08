#pragma once

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

class RmlUIIntegration
{
public:
    static bool Initialize(SDL_Window* window, SDL_Renderer* renderer);
    static void Cleanup();
    static void ProcessEvent(const SDL_Event& event, SDL_Window* window);
    static void Update();
    static void Render();
    
    static Rml::Context* GetContext() { return context; }

private:
    static Rml::Context* context;
    static bool initialized;
};