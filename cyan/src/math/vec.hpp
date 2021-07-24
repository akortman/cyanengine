#pragma once


namespace cyan {
    template <typename T>
    struct TVec3 {
        TVec3(): x{}, y{}, z{} {}
        TVec3(float x, float y, float z): x(x), y(x), z(z) {}
        T x;
        T y;
        T z;
    };

    using Vec3 = TVec3<float>;
}