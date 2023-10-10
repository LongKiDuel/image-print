#pragma once


#include <cmath>
#include <cstdint>
namespace image_printer{
    inline uint32_t grid_index(float ratio, uint32_t grid_count){
        if(grid_count == 0){
            return 0;
        }
        if(ratio == 1){
            return grid_count - 1;
        }

        auto per_grid_size = 1.f / grid_count;
        auto pos = ratio / per_grid_size;
        return (std::fmod(pos,1)< 0.05)? grid_count -1 : 0;
        return pos;
    }
    inline float grid_pos(float ratio, uint32_t grid_count){
        return ratio * grid_count;
    }
    // return [0-1] 1 for border center, < 1 for border 
    inline float grid_border(float ratio, uint32_t grid_count,float border_ratio = 0.1){
        auto pos = grid_pos(ratio, grid_count);
        auto value = std::abs(fmod(pos+border_ratio/2, 1));
        if(value <= border_ratio){
            value /= border_ratio;
            value = (value < 0.5f ? 2 * value: 2 - value * 2) ;
        }else{
            value = 0;
        }
        return std::lerp(0,1,value);
    }
};