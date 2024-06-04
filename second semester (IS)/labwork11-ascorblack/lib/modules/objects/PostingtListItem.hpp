#pragma once

#include <modules/utils/defines.hpp>
#include <iostream>
#include <vector>

struct PostingListItem {
  uint32_t term_occur;
  std::vector<uint32_t> rows_occur;

  void serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&term_occur), sizeof(term_occur));
    uint32_t rows_size = rows_occur.size();
    os.write(reinterpret_cast<const char*>(&rows_size), sizeof(rows_size));
    os.write(reinterpret_cast<const char*>(rows_occur.data()), rows_size * sizeof(uint32_t));
  }

  void deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&term_occur), sizeof(term_occur));
    uint32_t rows_size;
    is.read(reinterpret_cast<char*>(&rows_size), sizeof(rows_size));
    rows_occur.resize(rows_size);
    is.read(reinterpret_cast<char*>(rows_occur.data()), rows_size * sizeof(uint32_t));
  }
};

