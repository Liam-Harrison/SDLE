#define RMLUI_SDL_VERSION_MAJOR 3
#include "SDLE/RmlUI.h"
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>

// Include the SDL backend files from our local copy
#include "Backends/RmlUi_Platform_SDL.h"
#include "Backends/RmlUi_Renderer_SDL.h"

Rml::Context* RmlUIIntegration::context = nullptr;
bool RmlUIIntegration::initialized = false;

bool RmlUIIntegration::Initialize(SDL_Window* window, SDL_Renderer* renderer)
{
    if (initialized)
        return true;

    static RenderInterface_SDL render_interface(renderer);
    static SystemInterface_SDL system_interface;

    system_interface.SetWindow(window);
    
    Rml::SetRenderInterface(&render_interface);
    Rml::SetSystemInterface(&system_interface);

    if (!Rml::Initialise())
        return false;

    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);
    
    context = Rml::CreateContext("main", Rml::Vector2i(window_width, window_height));
    
    if (!context)
    {
        Rml::Shutdown();
        return false;
    }

    Rml::Debugger::Initialise(context);

    initialized = true;
    return true;
}

void RmlUIIntegration::Cleanup()
{
    if (!initialized)
        return;

    Rml::Debugger::Shutdown();
    
    if (context)
        context = nullptr;
    
    Rml::Shutdown();
    initialized = false;
}

void RmlUIIntegration::ProcessEvent(const SDL_Event& event, SDL_Window* window)
{
    if (!initialized || !context)
        return;

    // Convert SDL events to RmlUI events using the platform backend
    SDL_Event mutable_event = event;
    RmlSDL::InputEventHandler(context, window, mutable_event);
}

void RmlUIIntegration::Update()
{
    if (!initialized || !context)
        return;

    context->Update();
}

void RmlUIIntegration::Render()
{
    if (!initialized || !context)
        return;

    context->Render();
}