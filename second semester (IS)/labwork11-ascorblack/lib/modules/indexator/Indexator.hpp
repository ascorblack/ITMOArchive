#pragma once

#include <modules/indexator/Indexator.hpp>
#include <modules/utils/FilesExtractor.hpp>
#include <modules/utils/defines.hpp>
#include <modules/iterators/StreamFileLines.h>
#include <modules/iterators/StreamWords.hpp>
#include <modules/utils/FilterWord.hpp>
#include <modules/objects/DocStorage.hpp>

class Indexer {
 private:
  PrefixTree* prefix_tree_{nullptr};
  DocStorage* doc_storage_{nullptr};
  const std::string path_to_dir_;
  const std::string path_to_stopwords_;
  const std::string path_to_bin_dir_;
  std::vector<fs::path> list_of_files_;
  WordFilter* word_filter_{nullptr};

  void fetchFilePaths() {
    list_of_files_ = get_files_recursively(path_to_dir_);
    PRINT_INFO("Будет обработано %d корректных файлов.", list_of_files_.size());
  }

  void Initialization() {
    word_filter_ = new WordFilter(path_to_stopwords_);
    fetchFilePaths();
    prefix_tree_ = new PrefixTree();
    doc_storage_ = new DocStorage();
  }

 public:

  bool indexation() {
    Initialization();

    doc_id_t doc_id = 1;
    uint64_t total_length = 0;
    for (const auto& file_path : list_of_files_) {
      auto progress = static_cast<double>(static_cast<double>(doc_id) / static_cast<double>(list_of_files_.size()));
      Document document{0, file_path};

      PRINT_PROGRESS("Прогресс: ", progress);

      uint32_t row_num = 1;
      for (const auto& line : StreamFileLines(file_path)) {
        for (auto word : StreamWords(line)) {
          if (!word_filter_->isCorrectWord(word))
            continue;

          to_lower(word);
          prefix_tree_->add(word, doc_id, row_num);
          document.doc_length++;
        }
        row_num++;
      }

      doc_storage_->doc_map[doc_id] = document;
      total_length += document.doc_length;
      doc_id++;
    }
    doc_storage_->avg_doc_length = static_cast<double>(total_length) / static_cast<double>(list_of_files_.size());

    PRINT_REPLACE("Индексация успешно завершена!\n");
    PRINT_INFO("Полученный индекс содержит %d уникальных слов.", prefix_tree_->size());

    return true;
  }

  void save_to_file(const std::string& path_to_dir) const {
    if (!fs::exists(path_to_dir) && !fs::is_directory(path_to_dir))
      fs::create_directory(path_to_dir);

    prefix_tree_->save_to_file(path_to_dir + index_bin_filename);
    doc_storage_->save_to_file(path_to_dir + doc_storage_bin_filename);
  }

  void load_from_file() {
    prefix_tree_ = new PrefixTree();
    doc_storage_ = new DocStorage();
    prefix_tree_->load_from_file(path_to_bin_dir_ + index_bin_filename);
    doc_storage_->load_from_file(path_to_bin_dir_ + doc_storage_bin_filename);
  }

  uint64_t countOfUniqueTerm() {
    return prefix_tree_->size();
  }

  explicit Indexer(std::string path_to_dir, std::string path_to_stopwords)
      : path_to_dir_(std::move(path_to_dir)), path_to_stopwords_(std::move(path_to_stopwords)) {};

  explicit Indexer(std::string path_to_bin_dir) : path_to_bin_dir_(std::move(path_to_bin_dir)) {
    load_from_file();
  };

  ~Indexer() {
    delete prefix_tree_;
    prefix_tree_ = nullptr;

    delete doc_storage_;
    doc_storage_ = nullptr;

    delete word_filter_;
    word_filter_ = nullptr;
  }

};




