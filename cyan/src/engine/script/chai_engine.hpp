#pragma once

#include "chaiscript/chaiscript.hpp"

namespace cyan {
    struct ChaiEngine {
        ChaiEngine() : chai() {}

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
        void run(const std::string& code);

        /**
         * Run a file of chai code.
         * TODO: this is a temporary function. In the future, chai code will be executed by running Script resources.
         * @param code The path to the file to run.
         */
        void run_file(const std::string& path);

    private:
        chaiscript::ChaiScript chai;
    };
}

