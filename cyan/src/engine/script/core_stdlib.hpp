#pragma once

#include "cyan/src/engine/ecs/ecs.hpp"
#include "chai_engine.hpp"

namespace cyan {
    /**
     * Contains functions for use when added to the chai engine.
     */
    namespace chai_impl {
        /// Get a substring of a string given start and end positions.
        std::string substring(const std::string& s, std::size_t start, std::size_t end) {
            return s.substr(start, end - start);
        }
    }

    /**
     * Add core cyanengine functionality to chai.
     * @param chai_engine The ChaiEngine to add the functions to.
     * @param logger_obj A logger object. Defaults to the global logger object. (TODO: dedicated Chai logger, perhaps?)
     */
    inline void chai_add_cyan_stdlib(
            cyan::ChaiEngine& chai_engine,
            cyan::Logger& logger_obj = cyan::logger) {
        auto& chai = chai_engine.get_chai_object();

        chai.add(chaiscript::user_type<Logger>(), "CyanLogger");
        chai.add(chaiscript::var(logger_obj), "cyan_logger");
        chai.add(chaiscript::fun(&Logger::log_script, &logger_obj), "cyan_log_string_impl");
        chai.add(chaiscript::fun(&chai_impl::substring), "substring");

        chai_engine.run_file("../cyan/script/stdlib.chai");
    }
}