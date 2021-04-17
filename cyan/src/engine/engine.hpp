/**
 * Contains the Engine core class, which is responsible for setup and teardown of all core engine components.
 * Other components are opt-in.
 */

#pragma once

#include "script/chai_engine.hpp"
#include "cyan/src/logging/logger.hpp"

namespace cyan {
    // Forward-declare engine so we can specify it as a friend to an
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

    private:
        EngineBuilder() = default;
        friend Engine;
    };

    /** Engine
     * Owns the core engine components, as well as any optional components specified at construction.
     * The core engine components are:
     *  - The Entity-Component architecture
     *  - The chaiscript engine
     * Optional components:
     *  - A logger
     */
    struct Engine {
        /**
         * Returns an EngineBuilder object which can be used to construct an Engine with specified components.
         * @return An EngineBuilder object
         */
        static EngineBuilder build_engine();

        ~Engine();

    private:
        Engine();

        friend EngineBuilder;

        ChaiEngine chai_engine;
    };
}
