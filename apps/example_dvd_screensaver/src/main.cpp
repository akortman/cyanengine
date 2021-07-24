/**
 * Example "DVD screensaver" replica.
 * This example demonstrates the use of custom systems to create change in a small Scene (one Entity).
 */

#include <cyan/generated/components/Transform.hpp>
#include "cyan/src/engine.hpp"

using namespace cyan;

struct DvdBounceSystem: public systems::System {
    explicit DvdBounceSystem(Scene& scene, float speed): System(0){
        auto dvd_entity = scene.ecs().new_entity();
        scene.ecs().add_component<component::Transform>(dvd_entity, Vec3(0.0, 0.0, 0.0));
        scene.ecs().add_component<component::Physics>(dvd_entity, Vec3(speed, 0.0, 0.0));
    }

    void update(Scene& scene, double ticks_elapsed) override {
        for (auto& e: scene.ecs().iter_components<component::Transform, components::Physics>()) {
            auto tf_entry = scene.ecs().get_component<component::Transform>(e);
            auto phys_entry = scene.ecs().get_component<component::Physics>(e);
            tf_entry->translate = tf_entry->translate + ticks_elapsed * phys_entry->velocity;
        }
    }
};

int main() {
    try {
        auto engine = cyan::Engine::build_engine()
                .with_logger(cyan::LogVerbosity::DEBUG, &std::cerr)
                .with_scene("Main Scene")
                .with_renderer_2d_opengl()
                .create();
        engine.active_scene().systems().add_system<DvdBounceSystem>(engine.active_scene(), 0.01);
        engine.run();
        return 0;
    } catch (cyan::Error& e) {
        LOG(FATAL, "Unhandled cyan::Error: {}", e.what());
        std::exit(EXIT_FAILURE);
    }
}

