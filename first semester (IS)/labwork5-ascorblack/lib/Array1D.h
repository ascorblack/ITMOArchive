#pragma once

#include "ArrayData.h"
#include "uint17_t.h"


class Array1D {
 public:
  explicit Array1D(ArrayData* array_data, uint16_t* begin_16, uint8_t* begin_8);

  uint17_t operator[](int32_t index);

 private:
  ArrayData* array_data_{};
  uint16_t* begin_16_{};
  uint8_t* begin_8_{};
};
