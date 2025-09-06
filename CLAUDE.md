# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build System

This project uses CMake with presets for Windows development:

### Building the Project
```bash
# Configure (Windows x64 Debug - most common)
cmake --preset=x64-debug

# Build
cmake --build out/build/x64-debug

# Available presets: x64-debug, x64-release, x86-debug, x86-release
```

### Running the Application
```bash
# After building, the executable will be in:
out/build/x64-debug/Debug/SDLE.exe
```

## Project Structure

- `source/` - Main application source code
  - `SDLE.cpp` - Main application entry point using SDL3 callback-based architecture
  - `Assets.h/cpp` - Asset management utilities for fonts and resources
- `resources/` - Asset files (automatically copied to output during build)
- `vendored/` - Third-party dependencies managed as git submodules:
  - SDL3 (core graphics/audio/input)
  - SDL3_ttf (font rendering)
  - SDL3_mixer (audio mixing)
  - SDL3_image (image loading)

## Architecture

The application uses SDL3's new callback-based architecture:
- `SDL_AppInit()` - Initialize SDL subsystems, create window/renderer
- `SDL_AppEvent()` - Handle input events (quit, keyboard, etc.)
- `SDL_AppIterate()` - Main render loop (called every frame)
- `SDL_AppQuit()` - Cleanup (SDL handles most cleanup automatically)

The main application context is stored in a global `AppContext` struct containing the window and renderer.

## Dependencies

All dependencies are vendored as git submodules. The CMake configuration:
- Disables unused SDL_mixer formats for faster builds
- Disables unused SDL_image formats (keeps PNG support)
- Uses shared libraries on most platforms, static on Apple/Emscripten
- Automatically copies resources to output directory

## Development Notes

- C++20 standard required
- Uses SDL3 (not SDL2) - note the different API patterns
- Cross-platform support for Windows, macOS, and Linux
- Resources are handled differently per platform (macOS bundles vs directory copying)