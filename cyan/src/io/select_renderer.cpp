#include "select_renderer.hpp"

#include "renderer_headless.hpp"
#include "./backends/opengl/renderer.hpp"
#include "./backends/sdl2/renderer.hpp"

std::unique_ptr<cyan::io::RendererInterface> cyan::io::select_renderer(const std::string& name)
{
    #define NOT_IMPLEMENTED_ERR throw Error("Renderer \"{}\" not available in compiled engine", name)
    if (name == "headless") {
        return std::make_unique<HeadlessRenderer>();
    } else if (name == "OpenGL") {
        #ifdef CYAN_BACKEND_OPENGL
            return std::make_unique<backends::opengl::Renderer>();
        #else
            NOT_IMPLEMENTED_ERR;
        #endif
    } else if (name == "SDL2") {
        #ifdef CYAN_BACKEND_SDL2
            return std::make_unique<backends::sdl2::Renderer>();
        #else
            NOT_IMPLEMENTED_ERR;
        #endif
    } else {
        throw Error("Renderer \"{}\" does not exist", name);
    }
    #undef NOT_IMPLEMENTED_ERR
}