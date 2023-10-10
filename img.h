#pragma once
struct Img {
  int width;
  int height;
  int channel;
  char *data() {
    if (!buffer) {
      buffer = new char[width * height * channel];
    }
    return buffer;
  }
  char *buffer{};
};