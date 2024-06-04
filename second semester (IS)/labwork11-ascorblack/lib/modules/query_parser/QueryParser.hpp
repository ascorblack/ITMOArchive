#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <modules/iterators/StreamLetter.hpp>
#include <modules/utils/FilterWord.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>


enum class TokenType {
  Identifier,
  OperatorAnd,
  OperatorOr,
  OpenParenthesis,
  CloseParenthesis,
  End
};

struct ParseResult {
  std::unordered_set<uint32_t> doc_ids;
  std::vector<std::string> words;
};

struct Token {
  TokenType type;
  std::string text;
  std::unordered_set<uint32_t> doc_ids;
};

class Lexer {
 public:

  explicit Lexer() {};

  explicit Lexer(const std::string& input, PrefixTree* prefix_tree) : string_(input) {
    iterator_ = new LetterIterator(input);
    end_ = new LetterIterator();
    prefix_tree_ = prefix_tree;
  }

  Token getNextToken() {
    while (std::isspace((**iterator_)[0]))
      nextChar();

    if (**iterator_ == "(") {
      nextChar();
      return {TokenType::OpenParenthesis, "("};
    }

    if (**iterator_ == ")") {
      nextChar();
      return {TokenType::CloseParenthesis, ")"};
    }

    if (is_end) {
      return {TokenType::End, ""};
    }

    std::string identifier;

    while (!std::isspace((**iterator_)[0]) && (**iterator_) != ")" && (**iterator_) != ")" && !is_end) {
      identifier += **iterator_;
      nextChar();
    }

    if (identifier == "AND") return {TokenType::OperatorAnd, identifier};
    else if (identifier == "OR") return {TokenType::OperatorOr, identifier};
    else {
      to_lower(identifier);
      Token token = {TokenType::Identifier, identifier};

      if (!prefix_tree_->contains(identifier))
        RUNTIME_ERROR("Слова " + identifier + " нет в индексе.");

      for (auto doc_id : prefix_tree_->get(identifier)->data)
        token.doc_ids.insert(doc_id.first);

      return token;
    }
  }

 private:
  LetterIterator* iterator_{nullptr};
  LetterIterator* end_{nullptr};
  std::string string_;
  PrefixTree* prefix_tree_{nullptr};
  bool is_end = false;

  void nextChar() {
    if ((++(*iterator_)) == *end_)
      is_end = true;

  }
};

class QueryParser {
 public:

  explicit QueryParser(PrefixTree* prefix_tree) : prefix_tree_(prefix_tree) {};

  ParseResult parse(const std::string& input) {
    lexer_ = new Lexer(input, prefix_tree_);
    current_token_ = lexer_->getNextToken();
    parseExpression();
    if (token_stack_.empty())
      RUNTIME_ERROR("Ошибка при парсинге запроса.");

    auto result_token = token_stack_.top();

    return ParseResult{
        result_token.doc_ids,
      {words_.begin(), words_.end()}
    };
  }

 private:
  Lexer* lexer_{nullptr};
  Token current_token_;
  PrefixTree* prefix_tree_;
  std::stack<Token> token_stack_;
  std::unordered_set<std::string> words_;

  void parseExpression() {
    parseTerm();
    while (current_token_.type == TokenType::OperatorOr) {
      Token op = current_token_;
      consume(TokenType::OperatorOr);
      parseTerm();
      Token left_token = token_stack_.top();
      token_stack_.pop();
      Token right_token = token_stack_.top();
      token_stack_.pop();
//      printf("%s - %s - %s\n", left_token.text.c_str(), op.text.c_str(), right_token.text.c_str());
      Token new_token{TokenType::OperatorOr, "(" + left_token.text + " " + op.text + " " + right_token.text + ")"};
      std::unordered_set<uint32_t> merged_set;

      for (uint32_t doc : left_token.doc_ids) merged_set.insert(doc);
      for (uint32_t doc : right_token.doc_ids) merged_set.insert(doc);

      new_token.doc_ids = merged_set;

      token_stack_.push(new_token);
    }
  }

  void consume(TokenType type) {
    if (current_token_.type == type) {
      current_token_ = lexer_->getNextToken();
    } else {
      RUNTIME_ERROR("Unexpected token");
    }
  }

  void parseTerm() {
    parseFactor();
    while (current_token_.type == TokenType::OperatorAnd) {
      Token op = current_token_;
      consume(TokenType::OperatorAnd);
      parseFactor();
      Token left_token = token_stack_.top();
      token_stack_.pop();
      Token right_token = token_stack_.top();
      token_stack_.pop();
//      printf("%s - %s - %s\n", left_token.text.c_str(), op.text.c_str(), right_token.text.c_str());
      Token new_token{TokenType::OperatorAnd, "(" + left_token.text + " " + op.text + " " + right_token.text + ")"};
      std::unordered_set<uint32_t> merged_set;

      for (uint32_t doc : left_token.doc_ids)
        if (right_token.doc_ids.contains(doc))
          merged_set.insert(doc);

      new_token.doc_ids = merged_set;

      token_stack_.push(new_token);
    }

  }

  void parseFactor() {
    if (current_token_.type == TokenType::OpenParenthesis) {
      consume(TokenType::OpenParenthesis);
      parseExpression();
      consume(TokenType::CloseParenthesis);
    } else if (current_token_.type == TokenType::Identifier) {
      token_stack_.push(current_token_);
      std::string text = current_token_.text;
      to_lower(text);
      words_.insert(text);
      consume(TokenType::Identifier);
    } else {
      RUNTIME_ERROR("Unexpected token in factor");
    }
  }
};

