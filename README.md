# SDLE

## Project Setup

1. Clone the repository:
```sh
git clone https://github.com/Liam-Harrison/SDLE --depth=1 --recurse-submodules
```

2. Install Vulkan SDK:
   - Download [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
   - Ensure the `VULKAN_SDK` environment variable is set.

3.1 Windows Only - If you haven't configured PowerShell to run scripts, open it as administrator and run:
```sh
Set-ExecutionPolicy -ExecutionPolicy Unrestricted
```

3.2 Run install scripts located in:
   - `vendored/SDL_image/external/`
   - `vendored/SDL_mixer/external`
   - `vendored/SDL_ttf/external`