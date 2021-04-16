/**
 * TODO
 * This will be a bare-bone initialization of the engine and running of a main loop.
 * Ideally, this will be as simple as two function calls: one to initialize cyanengine, and one to run it.
 * The actual game content should be ran via the game scripts.
 */

#include "cyan/src/engine/engine.hpp"

int main() {
    auto engine = cyan::Engine::build_engine()
            .with_logger(cyan::LogVerbosity::DEBUG, &std::cerr)
            .create();

    return 0;
}