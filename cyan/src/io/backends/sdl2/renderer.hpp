#pragma once

#include "cyan/src/io/renderer_interface.hpp"

#include "SDL2/include/SDL.h"

namespace cyan::io::backends::sdl2 {
    struct Renderer: public RendererInterface {
        /// Create a Renderer.
        Renderer();

        /// Draw the contents of the ECS onto the screen (will render according to what Entities have Render components)
        void draw(Scene& scene) override;

        const char* name() override {
            return "SDL2";
        }

        ~Renderer();

    private:
        SDL_Window* window = nullptr;
        SDL_Surface* screen_surface = nullptr;
        SDL_Renderer* sdl_renderer = nullptr;
    };
}