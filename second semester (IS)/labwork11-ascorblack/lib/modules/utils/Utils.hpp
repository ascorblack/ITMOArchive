#pragma once

#include <algorithm>
#include <vector>

bool isValidUtf8(const std::vector<unsigned char>& data) {
  int i = 0;
  while (i < data.size()) {
    int bytesToFollow = 0;
    unsigned char lead = data[i];

    if (lead <= 0x7F) {
      i++;
      continue;
    } else if (lead >= 0xC2 && lead <= 0xDF) {
      bytesToFollow = 1;
    } else if (lead >= 0xE0 && lead <= 0xEF) {
      bytesToFollow = 2;
    } else if (lead >= 0xF0 && lead <= 0xF4) {
      bytesToFollow = 3;
    } else {
      return false;
    }

    if (i + bytesToFollow >= data.size()) return false;

    for (int j = 1; j <= bytesToFollow; j++) {
      if (data[i + j] < 0x80 || data[i + j] > 0xBF) {
        return false;
      }
    }
    i += bytesToFollow + 1;
  }
  return true;
}


void to_lower_ascii(std::string& text) {
  std::locale loc;
  std::ranges::transform(text, text.begin(), [&](char c) {
    return std::tolower(c, loc);
  });
}

std::string to_lower_utf8(std::string& string) {
  auto s = string.data();

  for (auto* p = reinterpret_cast<unsigned char*>(s); *p; p++) {
    if (p[0] == 0xD0) {
      if (0x90 <= p[1] && p[1] <= 0x9F)
        p[1] += 32;
      else if (0xA0 <= p[1] && p[1] <= 0xAF)
        p[0] += 1, p[1] &= ~0x30;
      else if (p[1] == 0x81)
        p[0] = 0xD1, p[1] = 0x91;
    }
  }

  string = s;
  to_lower_ascii(string);

  return string;
}

void to_lower(std::string& string) {
  to_lower_ascii(string);
  string = to_lower_utf8(string);
}
