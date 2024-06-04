#pragma once

#include <unordered_map>
#include <modules/utils/defines.hpp>
#include <modules/objects/PostingtListItem.hpp>
#include <modules/iterators/StreamLetter.hpp>
#include <fstream>

struct TreeNode {
  std::unordered_map<std::string, TreeNode> next;
  bool is_terminal = false;
  std::unordered_map<doc_id_t, PostingListItem> data;

  explicit TreeNode() = default;
  explicit TreeNode(bool is_terminal) : is_terminal(is_terminal) {};

  ~TreeNode() = default;

  void serialize(std::ostream& os) const {
    uint32_t next_size = next.size();
    os.write(reinterpret_cast<const char*>(&next_size), sizeof(next_size));
    for (const auto& pair : next) {
      uint32_t key_length = pair.first.length();
      os.write(reinterpret_cast<const char*>(&key_length), sizeof(key_length));
      os.write(pair.first.c_str(), key_length);
      pair.second.serialize(os);
    }

    os.write(reinterpret_cast<const char*>(&is_terminal), sizeof(is_terminal));

    uint32_t data_size = data.size();
    os.write(reinterpret_cast<const char*>(&data_size), sizeof(data_size));
    for (const auto& pair : data) {
      os.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
      pair.second.serialize(os);
    }
  }

  void deserialize(std::istream& is) {
    uint32_t next_size;
    is.read(reinterpret_cast<char*>(&next_size), sizeof(next_size));
    for (uint32_t i = 0; i < next_size; ++i) {
      uint32_t key_length;
      is.read(reinterpret_cast<char*>(&key_length), sizeof(key_length));

      std::string key(key_length, '\0');
      is.read(&key[0], key_length);

      next[key] = TreeNode();
      next[key].deserialize(is);
    }

    is.read(reinterpret_cast<char*>(&is_terminal), sizeof(is_terminal));

    uint32_t data_size;
    is.read(reinterpret_cast<char*>(&data_size), sizeof(data_size));
    for (uint32_t i = 0; i < data_size; ++i) {
      doc_id_t doc_id;
      is.read(reinterpret_cast<char*>(&doc_id), sizeof(doc_id));

      PostingListItem item;
      item.deserialize(is);
      data[doc_id] = item;
    }
  }
};


class PrefixTree {
private:
  TreeNode root;
  uint64_t size_ = 0;

  void clear(TreeNode& node) {
    for (auto& child : node.next)
      clear(child.second);
  }

public:

  static void update(TreeNode* word_node, doc_id_t doc_id, uint32_t row_num) {
    word_node->data[doc_id].term_occur++;
    word_node->data[doc_id].rows_occur.push_back(row_num);
  }

  void add(const std::string& word, doc_id_t doc_id, uint32_t row_num) {
    auto word_node = get(word);
    if (word_node != nullptr && word_node->is_terminal)
      return update(word_node, doc_id, row_num);

    TreeNode* current_node = &root;

    for (const std::string& letter : StreamLetter(word)) {
      if (!current_node->next.contains(letter))
        current_node->next[letter] = TreeNode();

      current_node = &current_node->next[letter];
    }

    size_++;
    current_node->is_terminal = true;
    current_node->data[doc_id] = {1, {row_num}};
  }

  uint64_t size() const {
    return size_;
  }

  TreeNode* get(const std::string& word) {
    TreeNode* current_node = &root;

    for (const std::string& letter : StreamLetter(word)) {
      if (!current_node->next.contains(letter))
        return nullptr;

      current_node = &current_node->next[letter];
    }

    return current_node;
  }

  bool contains(const std::string& word) {
    auto word_node = get(word);
    return word_node != nullptr && word_node->is_terminal;
  }

  void save_to_file(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs)
      RUNTIME_ERROR("Не удалось открыть файл для записи индекса!");

    root.serialize(ofs);
    ofs.write(reinterpret_cast<const char*>(&size_), sizeof(size_));
  }

  void load_from_file(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs)
      RUNTIME_ERROR("Не удалось открыть файл для загрузки индекса!");

    root.deserialize(ifs);
    ifs.read(reinterpret_cast<char*>(&size_), sizeof(size_));
  }

  ~PrefixTree() = default;

};

