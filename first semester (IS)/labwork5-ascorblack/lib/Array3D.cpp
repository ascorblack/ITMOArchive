#include "Array3D.h"


Array3D::Array3D(int32_t depth, int32_t rows, int32_t cols) {
  uint64_t left_bits_size = depth * rows * cols;
  uint64_t last_bits_size = std::ceil(static_cast<double>(depth * rows * cols) / kUint8Length);

  array_data_ = new ArrayData{depth, rows, cols, left_bits_size, last_bits_size,
                              new uint16_t[left_bits_size]{0},
                              new uint8_t[last_bits_size]{0}};

}

Array3D Array3D::make_array(int32_t depth, int32_t rows, int32_t cols) {

  if (depth <= 0 || rows <= 0 || cols <= 0)
    throw std::bad_array_new_length();

  return Array3D(depth, rows, cols);
}

Array3D::~Array3D() {
  delete array_data_;
}

Array2D Array3D::operator[](int32_t depth) {

  if (depth < 0 || depth >= array_data_->depth)
    throw std::out_of_range("Index out of range!");

  uint64_t left_bits_shift = depth * array_data_->rows * array_data_->cols;
  uint64_t last_bits_shift = (depth * array_data_->rows * array_data_->cols) / kUint8Length;

  return Array2D(array_data_,
                 array_data_->begin_16 + left_bits_shift,
                 array_data_->begin_8 + last_bits_shift);
}

Array3D& Array3D::operator*(uint32_t rvalue) {
  Array3D* new_array_3d = new Array3D(array_data_->depth, array_data_->rows, array_data_->cols);

  for (uint64_t shift = 0; shift < array_data_->size; ++shift) {
    uint17_t left_number = GetValueAt(shift, array_data_);

    GetValueAt(shift, new_array_3d->array_data_) = left_number.GetValue() * rvalue;
  }

  return *new_array_3d;
}

Array3D& Array3D::operator+(const Array3D& rvalue) {

  if (rvalue.array_data_->depth != array_data_->depth ||
      rvalue.array_data_->cols != array_data_->cols ||
      rvalue.array_data_->rows != array_data_->rows)
    throw std::runtime_error("Arrays sizes do not match!");

  Array3D* new_array_3d = new Array3D(array_data_->depth, array_data_->rows, array_data_->cols);

  for (uint64_t shift = 0; shift < array_data_->size; ++shift) {
    uint17_t left_number = GetValueAt(shift, array_data_);
    uint17_t right_number = GetValueAt(shift, rvalue.array_data_);

    GetValueAt(shift, new_array_3d->array_data_) = left_number.GetValue() + right_number.GetValue();
  }

  return *new_array_3d;
}

Array3D& Array3D::operator-(const Array3D& rvalue) {

  if (rvalue.array_data_->depth != array_data_->depth ||
      rvalue.array_data_->cols != array_data_->cols ||
      rvalue.array_data_->rows != array_data_->rows)
    throw std::runtime_error("Arrays sizes do not match!");

  Array3D* new_array_3d = new Array3D(array_data_->depth, array_data_->rows, array_data_->cols);

  for (uint64_t shift = 0; shift < array_data_->size; ++shift) {
    uint17_t left_number = GetValueAt(shift, array_data_);
    uint17_t right_number = GetValueAt(shift, rvalue.array_data_);

    GetValueAt(shift, new_array_3d->array_data_) = left_number.GetValue() - right_number.GetValue();
  }

  return *new_array_3d;
}

uint17_t Array3D::GetValueAt(uint64_t shift, ArrayData* array_data) {
  return uint17_t{array_data->begin_16 + shift, array_data->begin_8 + (shift / kUint8Length), shift};
}

Array3D& Array3D::operator=(const Array3D &rvalue) {

  if (this == &rvalue)
    return *this;

  *array_data_ = *rvalue.array_data_;

  return *this;
}
