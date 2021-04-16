#pragma once

#include "chaiscript/chaiscript.hpp"

namespace cyan {
    struct ChaiEngine {
        ChaiEngine() : chai() {}
    private:
        chaiscript::ChaiScript chai;
    };
}