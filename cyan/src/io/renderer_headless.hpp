#pragma once

#include "renderer_interface.hpp"


namespace cyan::io {
    struct HeadlessRenderer: public RendererInterface {
        void draw(Scene& scene) override {}

        const char* name() override {
            return "headless";
        }
    };
}