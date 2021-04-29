#include "engine.hpp"

#include "cyan/src/engine/ecs/ecs_global.hpp"
#include "cyan/src/engine/script/core_stdlib.hpp"
#include "cyan/src/engine/script/ecs_script.hpp"
#include "cyan/src/engine/script/generated_script.hpp"
#include "cyan/src/io/backends/opengl/renderer.hpp"

using namespace cyan;

Engine EngineBuilder::create()
{
    return Engine(std::move(renderer_2d));
}


EngineBuilder& EngineBuilder::with_logger(cyan::LogVerbosity log_verbosity, std::ostream* output)
{
    cyan::logger.with_verbosity_threshold(log_verbosity);
    cyan::logger.add_output_stream(output);
    return *this;
}


EngineBuilder& EngineBuilder::with_renderer_2d_opengl()
{
    if (renderer_2d) throw cyan::Error("Attempt to create Engine with OpenGL 2D renderer, but a renderer has already "
                                       "been assigned.");
    renderer_2d = std::make_unique<io::backends::opengl::Renderer>();
    return *this;
}


EngineBuilder Engine::build_engine()
{
    return EngineBuilder();
}


Engine::Engine(std::unique_ptr<io::RendererInterface> renderer_2d)
        : renderer(std::move(renderer_2d))
{
    // Initialize the chai/cyan standard library contents.
    chai_add_cyan_stdlib(chai_engine);
    chai_add_ecs_library(chai_engine);
    chai_add_codegen_generated_library(chai_engine);

    LOG(INFO, "cyanengine initialized.");
}


Engine::~Engine()
{
    LOG(INFO, "cyanengine deinitialized.");
}


void Engine::run()
{
    while (true) {
        renderer->draw(ecs::global_ecs);
    }
}


