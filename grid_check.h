#pragma once


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
        return pos;
    }
};