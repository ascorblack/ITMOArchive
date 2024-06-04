#pragma once

#include <iostream>
#include <string>

template<typename... Args>
void LOGGING_PRINT(const std::string& text, const char* logging_type, const char* color, Args... args) {
  std::string fullText = std::string(logging_type) + color + text + "\n\033[0m";
  printf(fullText.c_str(), args...);
}

template<typename... Args>
void PRINT_INFO(const std::string& text, Args... args) {
  LOGGING_PRINT(text, "[INFO] ", "\033[36m", args...);
}

template<typename... Args>
void PRINT_WARNING(const std::string& text, Args... args) {
  LOGGING_PRINT(text, "[WARNING] ", "\033[33m", args...);
}

void PRINT_REPLACE(const char* text) {
  std::cout << "\r" << text;
}

void PRINT_PROGRESS(const char* prefix, double progress) {
  std::cout << "\r" << prefix << progress * 100 << "%";
}

void RUNTIME_ERROR(const std::string& text) {
  throw std::runtime_error("\033[31m" + text + "\033[36m");
}