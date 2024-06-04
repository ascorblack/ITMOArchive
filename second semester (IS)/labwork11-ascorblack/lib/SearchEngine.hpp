#pragma once

#include <modules/indexator/Indexator.hpp>
#include <modules/query_parser/QueryParser.hpp>
#include <cmath>

class BM25 {
 private:
  const double k = 0.7;
  const double b = 0.5;
  const double docs_avg_length_{};
  const uint32_t total_doc_count_{};

 public:

  [[nodiscard]] double calcIDF(uint32_t doc_term_found) const {
    return std::log(static_cast<double>(total_doc_count_) / doc_term_found);
  }

  [[nodiscard]] double calcScoreForTerm(double term_freq, double dl, uint32_t doc_found) const {
    double upper_expression = term_freq * (k + 1);
    double bottom_expression = term_freq + k * (1 - b + b * (dl / docs_avg_length_));
    double idf = calcIDF(doc_found);

    return upper_expression / bottom_expression * idf;
  }

  explicit BM25(double docs_avg_length, uint32_t total_doc_count)
      : docs_avg_length_(docs_avg_length), total_doc_count_(total_doc_count) {};

};

struct SearchResultItem {
  double score{};
  uint32_t doc_id{};
  std::string word;

  bool operator<(const SearchResultItem& other) const {
    return this->score < other.score;
  }

};

class SearchEngine {
 private:
  PrefixTree* prefix_tree_{nullptr};
  DocStorage* doc_storage_{nullptr};
  std::string path_to_bin_dir_{};
  uint32_t top_k_{};
  BM25* bm_25_{nullptr};
  QueryParser* query_parser_{nullptr};
  std::unordered_map<uint32_t, SearchResultItem> relevance_documents;

 public:

  void load_index() {
    printf("Загружаем индекс...");
    prefix_tree_ = new PrefixTree();
    doc_storage_ = new DocStorage();
    prefix_tree_->load_from_file(path_to_bin_dir_ + index_bin_filename);
    doc_storage_->load_from_file(path_to_bin_dir_ + doc_storage_bin_filename);
    printf("\tУспех!\n");
  }

  void Initialization() {
    load_index();
    bm_25_ = new BM25(static_cast<double>(doc_storage_->avg_doc_length), doc_storage_->doc_map.size());
    query_parser_ = new QueryParser(prefix_tree_);
  }

  void calcScoreForWord(const std::string& word, const std::unordered_set<uint32_t>& accessible_doc_ids) {

    if (!prefix_tree_->contains(word)) {
      printf("\n[WARNING] Слово %s - не содержится в индексе!\n", word.c_str());
      return;
    }

    TreeNode* term_node = prefix_tree_->get(word);
    for (const auto& [doc_id, posting_list] : term_node->data) {
      if (!accessible_doc_ids.contains(doc_id))
        continue;

      Document document = doc_storage_->doc_map[doc_id];
      double score_d_t = bm_25_->calcScoreForTerm(term_node->data[doc_id].term_occur,
                                                  static_cast<double>(document.doc_length),
                                                  term_node->data.size());

      if (relevance_documents.contains(doc_id))
        relevance_documents[doc_id].score += score_d_t;
      else
        relevance_documents[doc_id] = SearchResultItem{score_d_t, doc_id, word};

    }

  }

  void search(std::string& query) {
    ParseResult parse_result;

    try {
      parse_result = query_parser_->parse(query);
    } catch (std::exception& e) {
      std::string message = std::string{e.what()};
      if (message.contains("нет в индексе.")) {
        PRINT_WARNING(message.substr(5, message.length() - 5));
        return;
      }
      else {
        throw std::runtime_error(e.what());
      }
    }

    relevance_documents.clear();

    for (const auto& word : parse_result.words)
      calcScoreForWord(word, parse_result.doc_ids);

    std::vector<SearchResultItem> temp_results;
    for (const auto& item : relevance_documents)
      temp_results.emplace_back(item.second);

    std::sort(temp_results.begin(), temp_results.end());
    std::reverse(temp_results.begin(), temp_results.end());

    uint32_t top_x = 1;
    for (auto item : temp_results) {
      printf("%d:\tdoc_id - %d\tfile_path - %s\tscore - [%f]\nСписок строк со словом %s в этом файле: ",
             top_x,
             item.doc_id,
             doc_storage_->doc_map[item.doc_id].doc_name.c_str(),
             item.score, item.word.c_str());

      for (auto row : prefix_tree_->get(item.word)->data[item.doc_id].rows_occur)
        std::cout << row << ' ';

      std::cout << "\n";

      top_x++;
      if (top_x == top_k_)
        break;

    }

    std::destroy(temp_results.begin(), temp_results.end());
    relevance_documents.clear();
  }

  explicit SearchEngine(std::string path_to_bin_dir, uint32_t top_k)
      : path_to_bin_dir_(std::move(path_to_bin_dir)), top_k_(top_k) {
    Initialization();
  };

  ~SearchEngine() {
    delete prefix_tree_;
    prefix_tree_ = nullptr;
    delete doc_storage_;
    doc_storage_ = nullptr;
    delete bm_25_;
    bm_25_ = nullptr;
    delete query_parser_;
    query_parser_ = nullptr;
    relevance_documents.clear();
  }


};

