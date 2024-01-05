#include "Array3D.h"
#include "uint17_t.h"


uint17_t::uint17_t(uint16_t *left_bits_cell, uint8_t *last_bits_cell, uint64_t lindex) :
    left_bits_cell_(left_bits_cell), last_bits_cell_(last_bits_cell), lindex_(lindex) {

  value |= *left_bits_cell_;
  value |= (((0b1 << (lindex_ % kUint8Length)) & *last_bits_cell_) << (kLeftShift - lindex_ % kUint8Length));
}

uint17_t& uint17_t::operator=(uint32_t rvalue) {
  *left_bits_cell_ = static_cast<uint16_t>(rvalue & k16BitsMask);
  *last_bits_cell_ &= ~(1 << (lindex_ % kUint8Length));
  *last_bits_cell_ |= ((rvalue & k17BitMask) >> kLeftShift) << (lindex_ % kUint8Length);

  value = rvalue;

  return *this;
}

std::ostream& uint17_t::operator<<(std::ostream& os) const {
  os << value;
  return os;
}

std::ostream& operator<<(std::ostream& os, const uint17_t& rvalue) {
  os << rvalue.value;
  return os;
}

std::istream& operator>>(std::istream& is, uint17_t rvalue) {
  uint32_t value;
  is >> value;
  rvalue = value;
  return is;
}

bool uint17_t::operator==(uint32_t rvalue) const {
  return value == rvalue;
}

bool uint17_t::operator!=(uint32_t rvalue) const {
  return value != rvalue;
}

bool uint17_t::operator<(uint32_t rvalue) const {
  return value < rvalue;
}

bool uint17_t::operator>(uint32_t rvalue) const {
  return value > rvalue;
}

bool uint17_t::operator<=(uint32_t rvalue) const {
  return value <= rvalue;
}

bool uint17_t::operator>=(uint32_t rvalue) const {
  return value >= rvalue;
}

uint32_t uint17_t::GetValue() const {
  return value;
}

uint17_t& uint17_t::operator=(uint17_t rvalue) {
  *this = rvalue.GetValue();
  return *this;
}

uint17_t::operator uint32_t() const {
  return value;
}
