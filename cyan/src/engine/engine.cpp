#include "engine.hpp"

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
    LOG(INFO, "cyanengine initialized.");
}


