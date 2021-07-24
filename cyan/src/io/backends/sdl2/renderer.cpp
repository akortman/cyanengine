#include "renderer.hpp"

#include "cyan/generated/components/Render.hpp"
#include "cyan/generated/components/Transform.hpp"
#include "SDL2/include/SDL.h"

using namespace cyan;
using namespace cyan::io::backends::sdl2;


Renderer::Renderer()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw Error("Could not initialize SDL: {}", SDL_GetError());
    }

    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    window = SDL_CreateWindow(
            "TODO Window Name",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

    screen_surface = SDL_GetWindowSurface(window);
    sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


void Renderer::draw(Scene& scene)
{
    SDL_FillRect(screen_surface, nullptr, SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));

    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0x00);

    for (auto e: scene.ecs().iter_components<component::Render, component::Transform>()) {
        LOG(INFO, "drawing point");
        auto t = scene.ecs().get_component<component::Transform>(e);
        auto err = SDL_RenderDrawPoint(sdl_renderer, t->translate.x, t->translate.y);
        if (err != 0) LOG(WARN, "Failed to render point");
    }

    SDL_UpdateWindowSurface(window);
}


Renderer::~Renderer()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}
