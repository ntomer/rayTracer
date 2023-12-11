#include "rtweekend.hpp"

#include "color.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"

#include <iostream>

color ray_color(const ray& r, const hittable& world){
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)){
        return 0.5*(rec.normal + color(1,1,1));
    }

    vec3 unit_dir = unit_vector(r.direction());
    auto a = 0.5*(unit_dir.y() +1.0);
    return (1-a)*color(1,1,1) + a*color(0.5, 0.7, 1.0);
}

int main(){

    //image
    auto aspect_ratio_ideal = 16.0/9.0;
    int im_width = 400;
    
    //calculate the image height and ensure that it's at least 1.
    int im_height = static_cast<int>(im_width / aspect_ratio_ideal);
    im_height = (im_height<1)?1:im_height;

    //world
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5, -1),100));

    //camera
    auto focal_length = 1.0;
    auto viewport_h = 2.0;
    auto viewport_w = viewport_h * (static_cast<double>(im_width)/im_height);
    auto camera_center = point3(0,0,0);

    //calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_w, 0,0);
    auto viewport_v = vec3(0,-viewport_h, 0);

    //calculate pixel steps
    auto pixel_delta_u = viewport_u / im_width;
    auto pixel_delta_v = viewport_v / im_height;

    //calculate coordinate of upper left pixel
    auto viewport_upper_left = camera_center
                                - vec3(0,0,focal_length) - viewport_u/2 -viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);

    //render
    std::cout <<  "P3\n" << im_width << " " << im_height << "\n255\n";

    for (int j = 0; j<im_height; ++j){
        std::clog << "\rScanlines remaining: " << (im_height - j) << ' ' << std::flush;
        for (int i = 0; i<im_width; ++i){
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rFinished! \n";

}