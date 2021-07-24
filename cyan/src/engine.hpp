/**
 * Contains the Engine core class, which is responsible for setup and teardown of all core engine components.
 * Other components are opt-in.
 */

#pragma once

#include <optional>

#include "cyan/src/engine/ecs/ecs.hpp"
#include "cyan/src/io/renderer_interface.hpp"
#include "cyan/src/engine/resource/resource_manager.hpp"
#include "cyan/src/engine/script/chai_engine.hpp"
#include "cyan/src/logging/logger.hpp"
#include "cyan/src/engine/scene.hpp"

namespace cyan {
    // Forward-declare engine so we can specify it as a friend to EngineBuilder
    struct Engine;

    /** EngineBuilder
     * Used to provide a fluent construction interface for the engine itself.
     * Can only be constructed via cyan::Engine::new_engine().
     *
     * Example usage:
     *      cyan::Engine engine = cyan::Engine::build_engine()
     *          .with_logger(cyan::LogVerbosity::DEBUG, std::cerr)
     *          .with_...(...) // chain together fluent interface functions here
     *          .with_...(...)
     *          .create();
     */
    struct EngineBuilder {
        /**
         * Create an engine with setting specified via the fluent interface.
         * @return an Engine object with the specified values.
         */
        Engine create();

        /**
         * Create the engine with a scene in-place.
         * @param scene_name The name of the scene
         * @return A reference to the EngineBuilder itself to allow fluent chaining of function calls.
         */
        EngineBuilder& with_scene(const std::string& scene_name);

        /**
         * Add logging information to the engine's logger.
         * @param log_verbosity The highest level of verbosity to output. The verbosities are DEBUG, INFO, WARN,
         *                      CRITICAL, and FATAL. DEBUG will show everything, while choosing CRITICAL will only show
         *                      CRITICAL and FATAL logs.
         * @param output A pointer to c++ stdlib output stream to output the log to. Suggested options are std::cerr or
         *               a file (or both - multiple outputs are supported, you'll just have to add them via the logger
         *               interface itself). TODO: Allow better control over logger initialization via EngineBuilder
         * @return A reference to the EngineBuilder itself to allow fluent chaining of function calls.
         */
        EngineBuilder& with_logger(cyan::LogVerbosity log_verbosity, std::ostream* output);

        /**
         * Attach a renderer to the Engine.
         * @return A reference to the EngineBuilder itself to allow fluent chaining of function calls.
         */
        EngineBuilder& with_renderer(const std::string& renderer_name);

        /**
         * Choose whether to initialize the script standard library.
         * @param b whether to initialize the chai stdlib
         */
        EngineBuilder& with_chai_stdlib(bool b);

    private:
        EngineBuilder() = default;
        friend Engine;

        std::string renderer = "headless";
        bool init_chai_stdlib = true;
        std::optional<std::string> init_scene_name;
    };

    /** Engine
     * Owns the core engine components, as well as any optional components specified at construction.
     * The core engine components are:
     *  - The Entity-Component architecture
     *  - The chaiscript engine
     * Optional components:
     *  - A logger
     */
    struct Engine: public GarbageCollectedContainer {
        /**
         * Returns an EngineBuilder object which can be used to construct an Engine with specified components.
         * @return An EngineBuilder object
         */
        static EngineBuilder build_engine();

        /**
         * Run the engine main loop until an exit signal is received.
         */
        [[noreturn]] void run();

        /**
         * Get the underlying ResourceManager.
         */
        ResourceManager& resources();

        /**
         * Create a new scene. Throws an error if there is a scene currently in use.
         * @return The newly created Scene object
         */
        Scene& new_scene(const std::string& scene_name);

        Scene& active_scene();

        /// Engine cannot be default-constructed. The only way to construct an Engine is via an EngineBuilder.
        Engine() = delete;

        /// Deconstructor.
        ~Engine();

        /**
         * Run a garbage collection cycle.
         * @param generator A random_device to select random elements
         * @param iters the scale of iterations (this will be fed to each data structure that can be garbage collected,
         *              and that many elements will be checked in each array).
         */
        void gc(std::random_device& generator, int iters) override {
            //ecs.gc(generator, iters)
            resource_manager.gc(generator, iters);
        }

    private:
        friend EngineBuilder;

        // TODO: I want to inject the actual Renderer here to allow Renderer settings to be easily passed into
        //       EngineBuilder's with_renderer() function
        Engine(const std::string& renderer_name,
                std::optional<std::string> initial_scene_name,
                bool with_scripts);

        void init_script_engine();

        std::optional<Scene> current_scene;
        ResourceManager resource_manager;
        std::optional<ChaiEngine> chai_engine;
        std::unique_ptr<io::RendererInterface> renderer;
    };
}
