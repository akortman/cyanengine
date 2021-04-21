#include "engine.hpp"

#include "script/core_stdlib.hpp"
#include "script/ecs_script.hpp"
#include "script/generated_script.hpp"

using namespace cyan;

Engine EngineBuilder::create()
{
    // TODO
    return Engine();
}


EngineBuilder& EngineBuilder::with_logger(cyan::LogVerbosity log_verbosity, std::ostream* output)
{
    cyan::logger.with_verbosity_threshold(log_verbosity);
    cyan::logger.add_output_stream(output);
    return *this;
}


EngineBuilder Engine::build_engine()
{
    return EngineBuilder();
}


Engine::~Engine()
{
    LOG(INFO, "cyanengine deinitialized.");
}


Engine::Engine()
{
    // Initialize the chai/cyan standard library contents.
    chai_add_cyan_stdlib(chai_engine);
    chai_add_ecs_library(chai_engine);
    chai_add_codegen_generated_library(chai_engine);

    LOG(INFO, "cyanengine initialized.");
}


