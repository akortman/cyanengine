#pragma once

#include <memory>
#include <vector>
#include "system.hpp"
#include "cyan/src/engine/scene.hpp"


namespace cyan::systems {
    struct SystemsManager {
        template <typename SystemT, typename ...Args>
        SystemT& add_system(Args...) {
            // TODO: Insertion can be more efficient here, but it's not vital, as new systems will probably not be
            //       created frequently.
            systems.push_back(std::make_unique<SystemT>(std::forward(Args...)));
            std::sort(systems.begin(), systems.end(), [](auto a, auto b) { return a->priority() < b.priority(); })
        }

        void update(Scene& scene, double ticks_elapsed) {
            for (auto& system: systems) {
                system->update(scene, ticks_elapsed);
            }
        }

    private:
        std::vector<std::unique_ptr<systems::System>> systems;
    };
}