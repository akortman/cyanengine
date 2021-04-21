#pragma once

#include "chaiscript/chaiscript.hpp"
#include "cyan/generated/chai_bindings.hpp"
#include "chai_engine.hpp"



namespace cyan {
    /**
     * Load in the generated chai bindings from the codegen system
     * @param chai_engine The chai script engine to bind the generated bindings to.
     */
    extern void chai_add_codegen_generated_library(cyan::ChaiEngine& chai_engine);
}