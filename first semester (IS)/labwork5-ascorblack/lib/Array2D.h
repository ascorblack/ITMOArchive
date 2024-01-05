#pragma once

#include "Array1D.h"


class Array2D {
 public:
  explicit Array2D(ArrayData* array_data, uint16_t* begin_16, uint8_t* begin_8);

  Array1D operator[](int32_t row);

 private:
  ArrayData* array_data_{};
  uint16_t* begin_16_{};
  uint8_t* begin_8_{};
};
