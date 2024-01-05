#pragma once

#include "Array2D.h"


class Array3D {
 public:
  explicit Array3D(int32_t depth, int32_t rows, int32_t cols);

  static Array3D make_array(int32_t depth, int32_t rows, int32_t cols);

  ~Array3D();

  Array2D operator[](int32_t depth);

  Array3D& operator=(const Array3D& rvalue);
  Array3D& operator*(uint32_t rvalue);
  Array3D& operator+(const Array3D& rvalue);
  Array3D& operator-(const Array3D& rvalue);

 private:
  ArrayData* array_data_;

  uint17_t GetValueAt(uint64_t shift, ArrayData* array_data);
};
