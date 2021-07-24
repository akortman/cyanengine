#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <array>
#include <fmt/format.h>

// TODO: This should be set somewhere else eventually.
#ifndef CYAN_LOGGER_ENABLE
#define CYAN_LOGGER_ENABLE
#endif


namespace cyan {
    /// Verbosity levels for logging
    /// Each logged string has a verbosity attached, and less-important verbosities can be filtered out if unnecessary.
    enum class LogVerbosity {
        DEBUG,      // For fine-grained information to reconstruct program activity
        INFO,       // General information about program events
        WARN,       // Report issues that are concerning, but unlikely to crash the program
        CRITICAL,   // Report issues that are likely to crash the program, but that execution will still be attempted
        FATAL,      // Report issues that *will* crash the program. Intended to be used immediately before a std::exit or
        // throwing an exception without a catch.
        NUM_LEVELS, // A way to keep track of the number of verbosity levels.
    };

    /**
     * Logger provides an io for logging information for debugging.
     */
    struct Logger {
        /// Construct a logger.
        Logger();

        // We have a simple fluent io to set up the logger.
        /// Set the verbosity of the logger (filtering out all logs with verbosity level less than the specified level).
        /// Returns a reference to the Logger to allow for chained with_*(...) calls.
        Logger& with_verbosity_threshold(LogVerbosity verb_threshold);

        /// Add an output stream for the logger to output to.
        /// This can be called multiple times to set up multiple output streams, e.g. a file and an ingame console.
        /// Returns a reference to the Logger to allow for chained with_*(...) calls.
        Logger& with_output_stream(std::ostream* output);

        /// Add an output stream for the logger to output to.
        /// This can be called multiple times to set up multiple output streams, e.g. a file and an ingame console.
        void add_output_stream(std::ostream* output);

        /// Add a log with given verbosity, file and line information. Uses fmt-style format strings for ease of use.
        /// Prefer use of the LOGL(...) macro (see below) for conciseness.
        template<typename... Params>
        void log_line(LogVerbosity verbosity, const char* file, int line, const char* fmt, Params... args) const
        {
            log(verbosity, fmt::format("{}:{} {}", file, line, fmt), args...);
        }

        /// Add a log with specified verbosity. Uses fmt-style format strings for ease of use.
        /// Prefer use of the LOG(...) macro below for conciseness.
        template<typename... Params>
        void log(LogVerbosity verbosity, const std::string& fmt, Params... args) const
        {
            if (int(verbosity) < int(verbosity_threshold)) return;
            for (int i = 0; i < num_outputs; i += 1) {
                this->print_time(outputs[i]);
                (*outputs[i]) << this->verbosity_to_str(verbosity) << ": " << fmt::format(fmt, args...) << '\n';
            }
        }

        /// Log a single string.
        /// This exists to faciliate log calls from scripts, which do not support variadic arguments.
        void log_script(const std::string& s) const {
            log(LogVerbosity::INFO, "(Script) {}", s);
        }

        /// Get the current verbosity visibility threshold.
        LogVerbosity get_verbosity_threshold();

        /// Convert a LogVerbosity enum value to string for display.
        static const char* verbosity_to_str(LogVerbosity verbosity);

    private:
        LogVerbosity verbosity_threshold;
        static const std::uint8_t max_outputs = 4;
        std::uint8_t num_outputs;
        std::array<std::ostream*, max_outputs> outputs;

        /// Print the current timestamp to an output stream.
        static void print_time(std::ostream* out);
    };

    extern Logger logger;
}

/// If CYAN_LOGGER_ENABLE is on, define some helper macros LOG and LOGL.
/// Both are used with the same signature:
///     LOG(LogVerbosity, text, fmt_args...);
/// e.g.
///     LOG(INFO, "A thing just happened with message: '{}' and value {}", message, value);
/// If CYAN_LOGGER_ENABLE is not enabled, these are empty macros (no cost once compiled).
/// TODO: It would be nice to have a compile-time verbosity level: i.e. we set so any LOG(...) call with verbosity less
/// TODO: than WARN is no-cost once compiled. This can probably be done with `constexpr if`
#ifdef CYAN_LOGGER_ENABLE
#ifndef LOG
#define LOG(VERB, ...) do { \
                    cyan::logger.log(cyan::LogVerbosity:: VERB, ##__VA_ARGS__); } \
                while (false)
#endif

#ifndef LOGL
#define LOGL(VERB, ...) do { \
                    cyan::logger.log_line( \
                        cyan::LogVerbosity:: VERB, __FILE__, __LINE__, ##__VA_ARGS__); } \
                while (false)
#endif
#else
#define LOG(VERB, ...)
#define LOGL(VERB, ...)
#endif