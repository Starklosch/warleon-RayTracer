#pragma once
#include <types.hpp>
#include <algorithm>

namespace war{
typedef struct Color{
    unsigned char r; // Red component
    unsigned char g; // Green component
    unsigned char b; // Blue component
    unsigned char a; // Alpha component

    // Constructor to initialize the color
    Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}

    Color(const Color& other)
        : r(other.r), g(other.g), b(other.b), a(other.a) {}

    Color operator*(vec_t intensity){
        return Color(
           std::clamp(r *int(intensity.x),0,255),
           std::clamp(g *int(intensity.y),0,255),
           std::clamp(b *int(intensity.z),0,255),
           a
        );
    }

}color_t;
};
