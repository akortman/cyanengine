/**
 * TODO
 * This will be a bare-bone initialization of the engine and running of a main loop.
 * Ideally, this will be as simple as two function calls: one to initialize cyanengine, and one to run it.
 * The actual game content should be ran via the game scripts.
 */

#include "cyan/src/engine.hpp"

#include "cyan/generated/components/components.hpp"
using namespace cyan;

int main() {
    try {
        auto engine = cyan::Engine::build_engine()
                .with_logger(cyan::LogVerbosity::DEBUG, &std::cerr)
                .with_scene("Main Scene")
                .with_renderer("SDL2")
                .create();
        auto e = engine.active_scene().ecs().new_entity();
        engine.active_scene().ecs().emplace_component<component::Transform>(e, Vec3(100, 100, 0));
        engine.active_scene().ecs().emplace_component<component::Render>(e);
        engine.run();
        return 0;
    } catch (cyan::Error& e) {
        LOG(FATAL, "Unhandled cyan::Error: {}", e.what());
        std::exit(EXIT_FAILURE);
    }
}