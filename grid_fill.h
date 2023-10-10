#pragma once


#include "image_printer.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
namespace image_printer{
    template <Image T,typename U,size_t N>
    inline void grid_index_paint(T & image, uint32_t grid_count_x,uint32_t grid_count_y,uint32_t pos_x,uint32_t pos_y, std::span<U,N> color){
        const auto width = image.width;    
        const auto height =image.height;
        auto grid_w = width / grid_count_x;
        auto grid_h = height / grid_count_y;

        const auto channel = image.channel;
        const auto data = image.data();
        auto set = [&](uint32_t x, uint32_t y){
            const auto base = data + (x+y*width) * channel;
            memcpy(base, color.data(), color.size());
        };
        const auto base_x = grid_w * pos_x;
        const auto base_y = grid_h * pos_y;

        const auto size_x = grid_w;
        const auto size_y = grid_h;

        for(auto y = base_y; y < base_y + size_y ;y++){
            for(auto x = base_x; x < base_x + size_x;x++){
                set(x,y);
            }
        }
    }
};