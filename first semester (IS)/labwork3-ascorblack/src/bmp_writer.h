#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>

#include "sandy_matrix.h"
#include "args_parser.h"

#pragma pack(push, 1)


enum class Color {
  White,
  Black,
  Purple,
  Yellow,
  Green
};


struct BMPHeader {
  uint16_t type;
  uint32_t size;
  uint16_t reserved1;
  uint16_t reserved2;
  uint32_t offset;
  uint32_t header_size;
  int32_t width;
  int32_t height;
  uint16_t planes;
  uint16_t bit_count;
  uint32_t compression;
  uint32_t image_size;
  int32_t x_pixels_per_meter;
  int32_t y_pixels_per_meter;
  uint32_t colors_used;
  uint32_t colors_important;
};


struct RGBPalette {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t reserved;

  void SetRGB(uint8_t R, uint8_t G, uint8_t B) {
    this->red = R;
    this->green = G;
    this->blue = B;
    this->reserved = 0;
  }

};


#pragma pack(pop)


class BMPWriter {
 private:
  int out_path_size = 4096;
  char* output_path;
  Options options_;
  uint8_t row_addition{};
  BMPHeader header{};
  RGBPalette palette[16]{};

  void CreateBMPHeader(int32_t width, int32_t height);
  void CreateRGBPalette();
  void GenerateOutputPath(int32_t step);
  void SetPaletteColor(int32_t index, Color color);

 public:

  explicit BMPWriter(Options user_arguments_);

  void Write(SandyMatrix* sandy_matrix);

  ~BMPWriter() {
    delete[] output_path;
  }

};
