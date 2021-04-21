#pragma once

#include <utility>

#include "chaiscript/chaiscript.hpp"

namespace cyan {
    struct ChaiEngine {
        /// Construct a ChaiEngine.
        ChaiEngine() : chai() {}

        /**
         * RunReport is returned by functions that execute chai code.
         * Errors will always result in a log warning, but if you want to ensure that a script ran correctly,
         * you can check the status of a RunReport.
         */
        struct RunReport {
            /// `ok` is true unless the script execution encountered an error.
            bool ok;

            /// If ok is false, `what` will contain the details of the error.
            std::string what;

            /// Create a RunReport.
            RunReport() : ok(true), what() {}
            RunReport(bool ok, std::string what) : ok(ok), what(std::move(what)) {}

            /// Throw an informative cyan::Error if the RunReport contains an error, and otherwise do nothing.
            void ensure_ok() const;
        };

        /**
         * Get the underlying chaiscript::Chaiscript object for direct manipulation. Necessary (at this stage) to add
         * functions, variables, etc. to the engine. (cyanengine provides it's own functions to run chaiscript code,
         * which offer protection from exceptions crashing the entire app).
         * @return A chaiscript::Chaiscript script engine object
         */
        chaiscript::ChaiScript& get_chai_object();

        /**
         * Run a string of chai code.
         * TODO: this is a temporary function. In the future, chai code will be executed by running Script resources.
         * @param code The code to run.
         */
        ChaiEngine::RunReport run(const std::string& code);

        /**
         * Run a file of chai code.
         * TODO: this is a temporary function. In the future, chai code will be executed by running Script resources.
         * @param code The path to the file to run.
         */
        ChaiEngine::RunReport run_file(const std::string& path);

    private:
        chaiscript::ChaiScript chai;
    };
}

