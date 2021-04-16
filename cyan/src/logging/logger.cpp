#include "logger.hpp"

using namespace cyan;


Logger cyan::logger;

Logger::Logger()
        : verbosity_threshold(LogVerbosity::INFO),
          num_outputs(0),
          outputs{}
{}


const char* Logger::verbosity_to_str(LogVerbosity verbosity)
{
    switch (verbosity) {
        case LogVerbosity::DEBUG:
            return "DEBUG: ";
        case LogVerbosity::INFO:
            return "";
        case LogVerbosity::WARN:
            return "WARNING: ";
        case LogVerbosity::CRITICAL:
            return "CRITICAL: ";
        case LogVerbosity::FATAL:
            return "FATAL ERROR: ";
        default:
            return "";
    }
}


void Logger::print_time(std::ostream* out)
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    (*out) << "[" << std::put_time(std::localtime(&now), "%T") << "] ";
}


Logger &Logger::with_output_stream(std::ostream* output)
{
    add_output_stream(output);
    return *this;
}


Logger &Logger::with_verbosity_threshold(LogVerbosity verb_threshold)
{
    this->verbosity_threshold = verb_threshold;
    return *this;
}


void Logger::add_output_stream(std::ostream* output)
{
    outputs[num_outputs] = output;
    num_outputs += 1;
}


