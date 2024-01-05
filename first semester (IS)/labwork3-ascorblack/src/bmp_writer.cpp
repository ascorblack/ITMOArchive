#include <iostream>
#include <cstring>

#include "bmp_writer.h"


BMPWriter::BMPWriter(Options user_options) {
  output_path = new char[out_path_size]{0};
  CreateRGBPalette();
  this->options_ = user_options;
}


void BMPWriter::GenerateOutputPath(int32_t step) {
  auto now = std::chrono::system_clock::now();
  auto UTC = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

  int len = snprintf(output_path, out_path_size, "%s", this->options_.output_dir);

  len += snprintf(output_path + len, out_path_size, "%d", step);

  output_path[len++] = '_';

  len += snprintf(output_path + len, out_path_size, "%ld", UTC);

  std::strcpy(output_path + len, ".bmp");
}


void BMPWriter::CreateBMPHeader(int32_t width, int32_t height) {
  this->row_addition = (8 - (width % 8)) % 8;

  header.type = 0x4D42; // 'BM'

  if (row_addition >= 2)
    header.size = sizeof(BMPHeader) + sizeof(palette) + ((width + row_addition) * height) / 2;
  else
    header.size = sizeof(BMPHeader) + sizeof(palette) + (width * height) / 2;

  header.reserved1 = 0;
  header.reserved2 = 0;
  header.offset = sizeof(BMPHeader) + sizeof(palette);
  header.header_size = 40;
  header.width = width;
  header.height = height;
  header.planes = 1;
  header.bit_count = 4; // 4 бита на пиксель
  header.compression = 0;

  if (row_addition >= 2)
    header.image_size = ((header.width + row_addition) * header.height) / 2;
  else
    header.image_size = (header.width * header.height) / 2;

  header.x_pixels_per_meter = 0;
  header.y_pixels_per_meter = 0;
  header.colors_used = 0;
  header.colors_important = 0;
}

void BMPWriter::SetPaletteColor(int32_t index, Color color) {

  switch (color) {
    case Color::Black:
      palette[index].SetRGB(0, 0,0);
      return;
    case Color::White:
      palette[index].SetRGB(255, 255, 255);
      return;
    case Color::Purple:
      palette[index].SetRGB(128, 0, 128);
      return;
    case Color::Yellow:
      palette[index].SetRGB(0, 255, 255);
      return;
    case Color::Green:
      palette[index].SetRGB(0, 128, 0);
      return;
  }

}


void BMPWriter::CreateRGBPalette() {
  SetPaletteColor(0, Color::White);
  SetPaletteColor(1, Color::Green);
  SetPaletteColor(2, Color::Yellow);
  SetPaletteColor(3, Color::Purple);
  SetPaletteColor(4, Color::Black);
}

void BMPWriter::Write(SandyMatrix* sandy_matrix) {
  CreateBMPHeader(sandy_matrix->width, sandy_matrix->height);
  GenerateOutputPath(sandy_matrix->step);

  std::ofstream bmp_file(this->output_path, std::ios::binary);

  if (!bmp_file.is_open()) {
    perror("Не удалось открыть файл");
    return;
  }

  bmp_file.write(reinterpret_cast<char*>(&header), sizeof(header));
  bmp_file.write(reinterpret_cast<char*>(&palette), sizeof(palette));

  uint8_t two_pixel = 0;
  int shift = 4;
  char pad_symbol = 'Z';

  for (int y = sandy_matrix->height - 1; y >= 0; y--) {

    for (int x = 0; x < sandy_matrix->width; x++) {
      two_pixel |= (sandy_matrix->matrix[y][x].getColor() & 0x0F) << shift;

      if (shift == 0) {
        bmp_file.write(reinterpret_cast<char*>(&two_pixel), sizeof(uint8_t));
        two_pixel = 0;
        shift = 4;
      } else {
        shift = 0;
      }
    }

    if (shift != 4) {
      bmp_file.write(reinterpret_cast<char*>(&two_pixel), sizeof(uint8_t));
      two_pixel = 0;
      shift = 4;
    }

    if (row_addition >= 2)
      for (int i = 0; i < row_addition / 2; i++)
        bmp_file.write(reinterpret_cast<char*>(&pad_symbol), sizeof(char));

  }

  bmp_file.close();
}
