#pragma once

#include "rtweekend.hpp"
#include "color.hpp"
#include "hittable.hpp"
#include "material.hpp"

#include <iostream>

class camera {
    public:
    double aspect_ratio_ideal = 1.0;
    int im_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10; 

    void render (const hittable& world) {
        initialize();

        std::cout <<  "P3\n" << im_width << " " << im_height << "\n255\n";

        for (int j = 0; j<im_height; ++j){
            std::clog << "\rScanlines remaining: " << (im_height - j) << ' ' << std::flush;
            for (int i = 0; i<im_width; ++i){
                color pixel_color(0,0,0);
                for(int sample = 0; sample < samples_per_pixel; sample++){
                    ray r = get_ray(i,j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(std::cout, pixel_color,samples_per_pixel);
            }
        }
        std::clog << "\rFinished! \n";
    }


    private:
    int im_height;      //rendeder image height
    point3 center;      //Camera center
    point3 pixel00_loc; //location of pixel 0,0
    vec3 pixel_delta_u; //offset to pixel to the right
    vec3 pixel_delta_v; //offset to pixel below


    void initialize() {
        //calculate the image height and ensure that it's at least 1.
        im_height = static_cast<int>(im_width / aspect_ratio_ideal);
        im_height = (im_height<1)?1:im_height;
        
        center = point3(0,0,0);

        //viewport dimensions
        auto focal_length = 1.0;
        auto viewport_h = 2.0;
        auto viewport_w = viewport_h * (static_cast<double>(im_width)/im_height);

        //calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = vec3(viewport_w, 0,0);
        auto viewport_v = vec3(0,-viewport_h, 0);

        //calculate pixel steps
        pixel_delta_u = viewport_u / im_width;
        pixel_delta_v = viewport_v / im_height;

        //calculate coordinate of upper left pixel
        auto viewport_upper_left = 
                center - vec3(0,0,focal_length) - viewport_u/2 -viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);

    }

    ray get_ray(int i, int j) const {
        auto pixel_center = pixel00_loc + (i*pixel_delta_u) + (j*pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;
        
        return ray(ray_origin, ray_direction);
    }

    vec3 pixel_sample_square() const {
        //returns a random point in the square surrounding a pixel at the origin
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        hit_record rec;
        if (depth <= 0)
            return color(0,0,0);

        if (world.hit(r, interval(0.001,infinity), rec)) {
            ray scattered;
            color attenuation;
            if(rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
    }

};