#pragma once

#include <modules/utils/defines.hpp>
#include <iostream>

struct Document {
  doc_length_t doc_length{0};
  std::string doc_name;

  void serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&doc_length), sizeof(doc_length));
    uint32_t name_length = doc_name.length();
    os.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
    os.write(doc_name.c_str(), name_length);
  }

  void deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&doc_length), sizeof(doc_length));
    uint32_t name_length;
    is.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
    doc_name.resize(name_length);
    is.read(&doc_name[0], name_length);
  }

};
