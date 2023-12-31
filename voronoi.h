#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <span>
#include <vector>
namespace image_printer {
struct Voronoi_nearest_result {
  uint64_t id;
  float distance;
};
class Voronoi {
public:
  Voronoi(uint64_t grid_size) : grid_size(grid_size) {
    points = std::vector<std::vector<Point>>(grid_size,
                                             std::vector<Point>{grid_size});
    for (auto &row : points) {
      for (auto &col : row) {
        col = rand_point();
      }
    }
  }
  Voronoi_nearest_result find_nearest(float u, float v) {
    uint64_t dx = u * grid_size;
    uint64_t dy = v * grid_size;

    float offset_u = fmod(u * grid_size, 1);
    float offset_v = fmod(v * grid_size, 1);

    const Point this_point{offset_u, offset_v};
    auto safe_decrease = [](uint64_t i) {
      if (i == 0) {
        return i;
      }
      return i - 1;
    };
    float min_distance = 3;
    uint64_t peer_id = -1;
    for (uint64_t y = safe_decrease(dy); y <= dy + 1; y++) {
      for (uint64_t x = safe_decrease(dx); x <= dx + 1; x++) {
        if (x >= grid_size || y >= grid_size) {
          continue;
        }
        using Si = int64_t;
        Si ddx = (Si)x - dx;
        Si ddy = (Si)y - dy;
        auto point = points[y][x];
        point.x += ddx;
        point.y += ddy;

        auto distance = this->distance(this_point, point);
        if (distance <= min_distance) {
          auto id = id_from_2d_coord(x, y);
          min_distance = distance;
          peer_id = id;
        }
      }
    }
    Voronoi_nearest_result result;
    result.distance = min_distance;
    result.id = peer_id;
    return result;
  };
  float distance(float u, float v) { return find_nearest(u, v).distance; }

private:
  uint64_t id_from_2d_coord(uint64_t x, uint64_t y) {
    return y * grid_size + x;
  }
  struct Point {
    float x;
    float y;
  };
  static float distance(const Point &a, const Point &b) {
    return distance_in_manhattan(a, b);
  }
  static float distance_in_euclidean(const Point &a, const Point &b) {
    return std::sqrt(pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
  }
  static float distance_in_manhattan(const Point &a, const Point &b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
  }
  static float dice() { return static_cast<float>(rand()) / RAND_MAX; }
  static Point rand_point() { return Point{dice(), dice()}; }
  uint64_t grid_size;
  float size_of_a_grid;
  std::vector<std::vector<Point>> points;
};
} // namespace image_printer