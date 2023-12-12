#pragma once

#include "vec3.hpp"

#include <iostream>

using color = vec3;

inline double linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}

//accepts the sum of the color(0.0-1.0), with the number of samples
// calculates the average, scales the result to 8 bit, then outputs it to the stream;
void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {

    auto scale = 1.0 / samples_per_pixel;
    pixel_color *= scale;

    static const interval intensity(0.000, 0.999);
    auto r = intensity.clamp(linear_to_gamma(pixel_color.x()));
    auto g = intensity.clamp(linear_to_gamma(pixel_color.y()));
    auto b = intensity.clamp(linear_to_gamma(pixel_color.z()));

    out << static_cast<int>(256 * r) << ' '
        << static_cast<int>(256 * g) << ' '
        << static_cast<int>(256 * b) << '\n';
}