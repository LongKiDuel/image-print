#include "colors_convert.h"

#include "colors.h"
#include <algorithm>
namespace colors {
Hsb rgbToHsb(const Rgb_u8 &rgb) {
  float r = rgb.r() / 255.0f;
  float g = rgb.g() / 255.0f;
  float b = rgb.b() / 255.0f;

  float max = std::max({r, g, b});
  float min = std::min({r, g, b});

  float h = 0.0f;
  float s = 0.0f;
  float v = max;

  float diff = max - min;

  if (max != 0.0f) {
    s = diff / max;
  }

  if (diff != 0.0f) {
    if (max == r) {
      h = (g - b) / diff + (g < b ? 6.0f : 0.0f);
    } else if (max == g) {
      h = (b - r) / diff + 2.0f;
    } else if (max == b) {
      h = (r - g) / diff + 4.0f;
    }
    h /= 6.0f;
  }

  return Hsb(h * 360.0f, s * 100.0f, v * 100.0f);
}
Rgb_u8 hsbToRgb(const Hsb &hsb) {
  float h = hsb.h() / 360.0f;
  float s = hsb.s() / 100.0f;
  float v = hsb.b() / 100.0f;

  int i = int(h * 6.0f);
  float f = h * 6.0f - i;
  float p = v * (1.0f - s);
  float q = v * (1.0f - s * f);
  float t = v * (1.0f - s * (1.0f - f));

  float r = 0.0f, g = 0.0f, b = 0.0f;

  switch (i % 6) {
  case 0:
    r = v, g = t, b = p;
    break;
  case 1:
    r = q, g = v, b = p;
    break;
  case 2:
    r = p, g = v, b = t;
    break;
  case 3:
    r = p, g = q, b = v;
    break;
  case 4:
    r = t, g = p, b = v;
    break;
  case 5:
    r = v, g = p, b = q;
    break;
  }

  return Rgb_u8(r * 255.0f, g * 255.0f, b * 255.0f);
}
} // namespace colors