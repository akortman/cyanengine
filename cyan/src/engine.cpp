#include "engine.hpp"

#include "cyan/src/engine/script/core_stdlib.hpp"
#include "cyan/src/engine/script/ecs_script.hpp"
#include "cyan/src/engine/script/generated_script.hpp"
#include "cyan/src/io/backends/opengl/renderer.hpp"
#include "cyan/src/io/select_renderer.hpp"

using namespace std::string_literals;

using namespace cyan;

Engine EngineBuilder::create()
{
    return Engine(
            renderer,
            init_scene_name,
            init_chai_stdlib);
}


EngineBuilder& EngineBuilder::with_logger(cyan::LogVerbosity log_verbosity, std::ostream* output)
{
    cyan::logger.with_verbosity_threshold(log_verbosity);
    cyan::logger.add_output_stream(output);
    return *this;
}


EngineBuilder& EngineBuilder::with_renderer(const std::string& renderer_name)
{
    renderer = renderer_name;
    return *this;
}

EngineBuilder& EngineBuilder::with_chai_stdlib(bool b) {
    init_chai_stdlib = b;
    return *this;
}


EngineBuilder& EngineBuilder::with_scene(const std::string& scene_name)
{
    if (init_scene_name.has_value()) {
        LOG(WARN, "Attempted to initialize engine with scene \"{}\", but engine "
                  "already set to initialize with a scene with name \"{}\", using latest scene name",
                  scene_name, *init_scene_name);
    }

    init_scene_name = scene_name;
    return *this;
}


EngineBuilder Engine::build_engine()
{
    return EngineBuilder();
}


Engine::Engine(const std::string& renderer_name,
               std::optional<std::string> initial_scene_name,
               bool with_scripts)
        : renderer(io::select_renderer(renderer_name))
{
    if (with_scripts) {
        init_script_engine();
    }

    if (initial_scene_name.has_value()) {
        new_scene(*initial_scene_name);
    }

    LOG(INFO, "cyanengine initialized with:\n"
              "    - renderer: {}\n"
              "    - initial scene: {}\n"
              "    - chai engine {}\n"
              "    - logger displaying warnings {} and above",
        renderer->name(),
        (initial_scene_name.has_value()
            ?  fmt::format(R"("{}")", *initial_scene_name)
            : "NONE"s),
        with_scripts ? "ENABLED" : "DISABLED",
        Logger::verbosity_to_str(logger.get_verbosity_threshold()));
}


Engine::~Engine()
{
    LOG(INFO, "cyanengine deinitialized.");
}


[[noreturn]] void Engine::run()
{
    if (!current_scene.has_value()) {
        LOG(FATAL, "Engine cannot run with no scene active.");
        throw cyan::Error("Engine cannot run with no scene active.");
    }

    while (true) {
        renderer->draw(*current_scene);
    }
}


void Engine::init_script_engine()
{
    chai_engine.emplace();
    // Initialize the chai/cyan standard library contents.
    chai_add_cyan_stdlib(*chai_engine);
}


ResourceManager& Engine::resources()
{
    return resource_manager;
}


Scene& Engine::new_scene(const std::string& scene_name)
{
    if (current_scene.has_value()) {
        throw cyan::Error(R"(Attempted to create a new Scene "{}", but a scene "{}" is already active)",
                          scene_name, current_scene->name());
    }
    current_scene = Scene(scene_name);

    if (chai_engine.has_value()) {
        chai_add_ecs_library(*chai_engine, current_scene->ecs());
        chai_add_codegen_generated_library(*chai_engine);
    }

    LOG(INFO, R"(Scene "{}" created and initialized)", scene_name);
    return *current_scene;
}


Scene& Engine::active_scene()
{
    return *current_scene;
}


