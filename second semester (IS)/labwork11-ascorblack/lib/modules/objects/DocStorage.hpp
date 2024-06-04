#pragma once

#include <modules/objects/Document.hpp>
#include <unordered_map>

struct DocStorage {
  std::unordered_map<doc_id_t, Document> doc_map;
  doc_avg_length_t avg_doc_length;

  void serialize(std::ostream& os) const {
    uint32_t map_size = doc_map.size();
    os.write(reinterpret_cast<const char*>(&map_size), sizeof(map_size));
    for (const auto& pair : doc_map) {
      os.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
      pair.second.serialize(os);
    }
    os.write(reinterpret_cast<const char*>(&avg_doc_length), sizeof(avg_doc_length));
  }

  void deserialize(std::istream& is) {
    uint32_t map_size;
    is.read(reinterpret_cast<char*>(&map_size), sizeof(map_size));
    for (uint32_t i = 0; i < map_size; ++i) {
      doc_id_t doc_id;
      is.read(reinterpret_cast<char*>(&doc_id), sizeof(doc_id));
      Document doc;
      doc.deserialize(is);
      doc_map[doc_id] = doc;
    }
    is.read(reinterpret_cast<char*>(&avg_doc_length), sizeof(avg_doc_length));
  }

  void save_to_file(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs)
      throw std::runtime_error("Cannot open file for writing");

    (*this).serialize(ofs);
  }

  void load_from_file(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs)
      throw std::runtime_error("Cannot open file for reading");

    (*this).deserialize(ifs);
  }

};
