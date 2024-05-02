#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>


namespace sf{

    inline const float scale = 50.f; // scale the physics
    template <typename T>
    inline Vector2<T> pixel_to_meter(const Vector2<T> &vec){
        return Vector2<T>(vec.x /scale, vec.y/scale);
    }
    template <typename T>
    inline Vector2<T> meter_to_pixel(const Vector2<T> &vec){
        return Vector2<T>(vec.x*scale, vec.y*scale);
    }
    template <typename T>
    inline T pixel_to_meter(T x){
        return static_cast<T>(x/scale);
    }
    template <typename T>
    inline T meter_to_pixel(T x){
        return static_cast<T>(x*scale);
    }

    // shortcut function

    template <typename T>
    inline Vector2<T> PtoM(const Vector2<T> &vec){
        return pixel_to_meter(vec);
    }
    template <typename T>
    inline Vector2<T> MtoP(const Vector2<T> &vec){
        return meter_to_pixel(vec);
    }
    template <typename T>
    inline T PtoM(T x){
        return pixel_to_meter(x);
    }
    template <typename T>
    inline T MtoP(T x){
        return meter_to_pixel(x);
    }
}