#include "number.h"

const int kClassNumberLength = 253;
const int kClassBase = 256;
const int kBitNegativeCheck = 128;

void fill_int2023(int2023_t* instance, const uint8_t val) {

  for (int i = 0; i < kClassNumberLength; ++i) {
    instance->bytes[i] = static_cast<uint8_t>(val);
  }

}

int get_buff_len(const char* buff) {
  int length = 0;

  while (buff[length] != '\0') length++;

  return length;
}

bool is_negative(int2023_t instance) {
  return (instance.bytes[kClassNumberLength - 1] & kBitNegativeCheck);
}

void set_negative(int2023_t instance) {
  instance.bytes[kClassNumberLength - 1] |= kBitNegativeCheck;
}

void abs_int2023(int2023_t instance) {
  if (is_negative(instance))
    instance.bytes[kClassNumberLength - 1] -= kBitNegativeCheck;
}

int get_len_int2023(int2023_t value) {
  int senior_grade = kClassNumberLength - 1;

  if (value.bytes[senior_grade] == kBitNegativeCheck) --senior_grade;

  while (value.bytes[senior_grade] == 0) --senior_grade;

  return senior_grade + 1;
}

int2023_t null_int2023() {
  int2023_t new_value;
  fill_int2023(&new_value, 0);

  return new_value;
}

int2023_t from_int(int32_t num) {
  int2023_t new_number;
  fill_int2023(&new_number, 0);
  int i = 0;
  bool num_negative = false;

  if (num < 0) {
    num = -num;
    num_negative = true;
  }

  while (num) {
    new_number.bytes[i] = static_cast<uint8_t>(num % kClassBase);
    num /= kClassBase;
    ++i;
  }

  if (num_negative) {
    set_negative(new_number);
  }

  return new_number;
}

int2023_t from_string(const char* buff) {
  int2023_t new_number;
  fill_int2023(&new_number, 0);

  int start = buff[0] == '-' ? 1 : 0;

  int length = get_buff_len(buff);
  for (int i = start; i < length; ++i) {
    int32_t carry = buff[i] - '0';

    for (int j = 0; j < kClassNumberLength; ++j) {
      carry += 10 * new_number.bytes[j];
      new_number.bytes[j] = static_cast<uint8_t>(carry % kClassBase);
      carry /= kClassBase;
    }
  }

  if (start == 1)
    set_negative(new_number);

  return new_number;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
  int carry = 0;
  int new_num_index = 0;

  bool lhs_neg = is_negative(lhs);
  bool rhs_neg = is_negative(rhs);

  if (rhs_neg && lhs_neg) {
    abs_int2023(rhs);
  } else if (lhs_neg) {
    abs_int2023(lhs);

    if (lhs < rhs) {
      return rhs - lhs;
    } else if (lhs == rhs) {
      return null_int2023();
    } else {
      int2023_t new_value = lhs - rhs;

      set_negative(new_value);
      return new_value;
    }
  } else if (rhs_neg) {
    abs_int2023(rhs);

    if (lhs < rhs) {
      int2023_t new_value = rhs - lhs;

      set_negative(new_value);
      return new_value;
    } else if (lhs == rhs)
      return null_int2023();
    else
      return lhs - rhs;

  }

  for (int i = 0; i < kClassNumberLength; ++i) {

    int buff = static_cast<int>(lhs.bytes[i]) + static_cast<int>(rhs.bytes[i]) + carry;

    lhs.bytes[new_num_index] = static_cast<uint8_t>(buff % kClassBase);
    carry = buff / kClassBase;
    ++new_num_index;

    if (carry > 0 && i == kClassNumberLength - 1)
      throw std::overflow_error("Числа слишком большие");

  }

  return lhs;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
  int carry = 0;

  bool lhs_neg = is_negative(lhs);
  bool rhs_neg = is_negative(rhs);

  if ((lhs_neg && rhs_neg) || rhs_neg) {
    abs_int2023(rhs);
    return lhs + rhs;
  } else if (lhs_neg) {
    set_negative(rhs);
    return lhs + rhs;
  }

  for (int i = 0; i < kClassNumberLength; ++i) {
    int diff = static_cast<int>(lhs.bytes[i]) - static_cast<int>(rhs.bytes[i]) - carry;

    if (diff < 0) {
      diff += kClassBase;
      carry = 1;
    } else {
      carry = 0;
    }

    lhs.bytes[i] = diff;
  }

  return lhs;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
  int len_1 = get_len_int2023(lhs);
  int len_2 = get_len_int2023(rhs);

  int2023_t new_value;
  fill_int2023(&new_value, 0);

  bool result_is_negative = false;

  if (!(is_negative(lhs) && is_negative(rhs)) && (is_negative(lhs) || is_negative(rhs))) {
    result_is_negative = true;
  }
  abs_int2023(lhs);
  abs_int2023(rhs);

  for (int i = 0; i < len_1; ++i) {
    int carry = 0;

    for (int j = 0; j < len_2; ++j) {
      int buff = static_cast<uint8_t>(new_value.bytes[i + j])
          + static_cast<uint8_t>(lhs.bytes[i]) * static_cast<uint8_t>(rhs.bytes[j]) + carry;
      carry = buff / kClassBase;
      new_value.bytes[i + j] = buff % kClassBase;
    }

    int sub_i = len_2;

    while (carry) {
      new_value.bytes[i + sub_i] = carry % kClassBase;
      ++sub_i;
      carry /= kClassBase;
    }

  }

  if (result_is_negative)
    set_negative(new_value);

  return new_value;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
  int len_1 = get_len_int2023(lhs);
  int len_2 = get_len_int2023(rhs);

  bool lhs_is_neg = is_negative(lhs);
  bool rhs_is_neg = is_negative(rhs);

  bool result_is_neg = false;

  if (!(lhs_is_neg && rhs_is_neg) && (lhs_is_neg || rhs_is_neg)) {
    result_is_neg = true;
  }
  abs_int2023(lhs);
  abs_int2023(rhs);

  if (rhs == from_int(0))
    throw std::runtime_error("Деление на ноль!");

  if (rhs == from_int(1)) return lhs;

  if (len_2 > len_1 || !(rhs < lhs)) return null_int2023();

  if (rhs == lhs) return from_int(1);

  int2023_t result_value = null_int2023();

  for (int i = len_1 - len_2 + 1; i >= 0; --i) {
    while (lhs >= rhs * result_value) {
      ++result_value.bytes[i];
    }
    --result_value.bytes[i];
  }

  if (result_is_neg) set_negative(result_value);

  return result_value;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {

  if (is_negative(lhs) && !is_negative(rhs) || !is_negative(lhs) && is_negative(rhs))
    return false;

  for (int i = 0; i < kClassNumberLength; ++i)
    if (lhs.bytes[i] != rhs.bytes[i])
      return false;

  return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {

  if (is_negative(lhs) && !is_negative(rhs) || !is_negative(lhs) && is_negative(rhs))
    return true;

  for (int i = 0; i < kClassNumberLength - 1; ++i)
    if (lhs.bytes[i] != rhs.bytes[i])
      return true;

  return false;
}

void operator<<(int2023_t& instance, int shift) {

  if (get_len_int2023(instance) == kClassNumberLength - 1) {
    throw std::overflow_error("<< is not possible");
  }

  int2023_t temp_shift = from_int(256);

  for (int i = 0; i < shift; ++i) {
    instance = instance * temp_shift;
  }

  delete[] temp_shift.bytes;
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
  int len_1 = get_len_int2023(lhs);
  int len_2 = get_len_int2023(rhs);

  if (len_1 < len_2) {
    return true;
  } else if (len_1 > len_2) {
    return false;
  } else {

    for (int i = len_1 - 1; i >= 0; --i) {

      if (lhs.bytes[i] > rhs.bytes[i]) {
        return false;
      }
      if (lhs.bytes[i] < rhs.bytes[i]) {
        return true;
      }

    }

    return true;
  }
}

bool operator>=(const int2023_t& lhs, const int2023_t& rhs) {
  int len_1 = get_len_int2023(lhs);
  int len_2 = get_len_int2023(rhs);

  if (is_negative(lhs) && !is_negative(rhs))
    return false;
  else if (!is_negative(lhs) && is_negative(rhs))
    return true;
  else if (is_negative(lhs) && is_negative(rhs))
    return lhs < rhs;

  if (len_1 < len_2) return false;

  for (int i = len_1; i >= 0; i--) {

    if (lhs.bytes[i] > rhs.bytes[i]) return true;
    else if (lhs.bytes[i] < rhs.bytes[i]) return false;

  }

  return true;
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
  int senior_grade = get_len_int2023(value) - 1;

  if (is_negative(value)) stream << "-";

  for (int i = senior_grade; i >= 0; --i) {
    stream << "(" << static_cast<int>(value.bytes[i]) << ")";

    if (i > 0) stream << " ";
  }

  return stream;
}