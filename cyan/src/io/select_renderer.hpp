#pragma once

#include <memory>
#include <string>
#include "renderer_interface.hpp"

namespace cyan::io {
    std::unique_ptr<RendererInterface> select_renderer(const std::string& name);
}