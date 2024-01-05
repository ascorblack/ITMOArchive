#pragma once

#include <iostream>


const uint32_t k16BitsMask = 0b1111111111111111;
const uint32_t k17BitMask = 0b10000000000000000;
const uint8_t kLeftShift = 16;

class uint17_t {
 public:

  explicit uint17_t(uint16_t* left_bits_cell, uint8_t* last_bits_cell, uint64_t lindex);

  uint32_t GetValue() const;

  bool operator==(uint32_t) const;
  bool operator!=(uint32_t) const;
  bool operator<(uint32_t) const;
  bool operator>(uint32_t) const;
  bool operator<=(uint32_t) const;
  bool operator>=(uint32_t) const;

  uint17_t& operator=(uint32_t rvalue);
  uint17_t& operator=(uint17_t rvalue);
  operator uint32_t() const;
  std::ostream& operator<<(std::ostream& os) const;
  friend std::ostream& operator<<(std::ostream& os, const uint17_t& rvalue);
  friend std::istream& operator>>(std::istream& is, uint17_t rvalue);

 private:
  uint32_t value{};

  uint16_t* left_bits_cell_;
  uint8_t* last_bits_cell_;
  uint64_t lindex_;
};
