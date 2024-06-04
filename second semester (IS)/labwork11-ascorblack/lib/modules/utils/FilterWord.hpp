#pragma once

#include <unordered_set>
#include <string>
#include <cctype>
#include <locale>
#include <codecvt>
#include <filesystem>

#include <modules/iterators/StreamWords.hpp>
#include <modules/iterators/StreamFileLines.h>
#include <modules/utils/PrefixTree.hpp>

class WordFilter {
 private:
  std::unordered_set<std::string> stop_words;

 public:

  explicit WordFilter(const std::filesystem::path& path_to_stopwords) {
    auto file = std::ifstream(path_to_stopwords);

    if (!file.is_open()) {
      PRINT_WARNING("При попытке открыть файл data/stopwords.txt произошла ошибка! "
                    "Список стоп слов не будет использоваться при индексации");
      return;
    }

    file.close();

    for (const auto& line : StreamFileLines(path_to_stopwords)) {
      for (const auto& word : StreamWords(line)) {
        stop_words.insert(word);
      }
    }

    PRINT_INFO("В словаре стоп-слов содержится %d термина.", stop_words.size());
  }


  bool isCorrectWord(const std::string& word) {
    return !stop_words.contains(word) && word.length() >= MIN_WORD_LENGTH
        && isValidUtf8(std::vector<unsigned char>{word.begin(), word.end()})
        && word.length() <= MAX_WORD_LENGTH;
  }

};
