#pragma once

#include <limits>

class interval {
    public:
    double min, max;

    //default interval is empty
    interval() : min(std::numeric_limits<double>::infinity()), 
                    max(-std::numeric_limits<double>::infinity()) {}

    interval(double _min, double _max) : min(_min), max(_max) {}

    bool contains(double x) const{
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const{
        if (x<min) return min;
        if (x>max) return max;
        return x;
    }

    static const interval empty, universe;

};

const static interval empty(std::numeric_limits<double>::infinity(), 
                            -std::numeric_limits<double>::infinity());
const static interval w(-std::numeric_limits<double>::infinity(), 
                            std::numeric_limits<double>::infinity());