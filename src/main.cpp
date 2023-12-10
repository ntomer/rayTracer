#include <iostream>
#include "color.hpp"
#include "vec3.hpp"

int main(){
    // Image metadata
    int im_width = 200;
    int im_height = 200;

    //create image
    std::cout <<  "P3\n" << im_width << " " << im_height << "\n255\n";

    for (int j = 0; j<im_height; ++j){
        for (int i = 0; i<im_width; ++i){
            std::clog << "\rScanlines remaining: " << (im_height - j) << ' ' << std::flush;

            auto pixel_color = color(double(i)/(im_width-1), double(j)/(im_height-1), 0);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rFinished! \n";

}