#pragma once
#include <cstdint>
#include <span>
namespace image_printer {
template <typename T> class Bimmap_iter {
public:
  struct Image_context;
  struct Row_context;
  struct Pixel_context;

public:
  Bimmap_iter(Image_context ctx) : ctx_(ctx) {}
  template <typename U> void for_each_pixel(U callback) {
    for (uint64_t i{}; i < ctx_.height; i++) {
      std::span<T> row_span = ctx_.buffer.subspan(ctx_.elems_pre_lines * i,
                                                  ctx_.channel * ctx_.width);
      Row_context row{ctx_, row_span, i};
      for (uint64_t j{}; j < row.row_buffer.size(); j++) {
        std::span<T> pixel_span =
            row.row_buffer.subspan(ctx_.channel * j, ctx_.channel);
        Pixel_context pixel{row, pixel_span, j};
        callback(pixel);
      }
    }
  }
  constexpr std::span<T> lookup_pixel(int x, int y) {
    if (x >= ctx_.width || y >= ctx_.height) {
      return {};
    }
    return ctx_.buffer.subspan(ctx_.elems_pre_lines * y + ctx_.channel * x,
                               ctx_.channel);
  }

public:
  struct Image_context {
    std::span<T> buffer;

    uint64_t width;
    uint64_t height;
    uint64_t elems_pre_lines; // equal to width if no pad.
    uint32_t channel;
  };

  struct Row_context {
    const Image_context &image;

    std::span<T> row_buffer; // there is no pad in this buffer
    uint64_t row_index;
  };

  struct Pixel_context {
    const Row_context &row_context;
    std::span<T> pixel_buffer;
    uint64_t col_index;

    uint64_t col() const { return col_index; }
    uint64_t row() const { return row_context.row_index; }
    uint64_t channel() const { return pixel_buffer; }
  };

private:
  Image_context ctx_;
};

template <typename T>
bool is_boundary(const typename Bimmap_iter<T>::Pixel_context &pixel) {
  auto col = pixel.col();
  auto row = pixel.row();
  auto &image = pixel.row_context.image;

  return col == 0 || row == 0 || col + 1 == image.width ||
         row + 1 == image.height;
}
} // namespace image_printer