#pragma once

#include "../../renderer_interface.hpp"


namespace cyan::draw2d::backends::opengl {
    struct Renderer: public RendererInterface {
        Renderer();

        void draw(ECS& ecs) override;
    };
}