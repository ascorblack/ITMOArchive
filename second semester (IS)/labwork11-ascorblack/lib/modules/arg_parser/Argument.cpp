#include "ArgParser.h"

using namespace ArgumentParser;


template<typename T>
Argument<T>& Argument<T>::Default(const T& default_value) {
  default_ = default_value;

  return *this;
}

template<typename T>
Argument<T> &Argument<T>::MultiValue(const size_t& min_count) {
  if (std::get_if<SingleDataType<T>>(&value_))
    value_ = MultiDataType<T>{};

  std::get<MultiDataType<T>>(value_).min_count = min_count;

  return *this;
}

template<typename T>
Argument<T> &Argument<T>::Positional() {
  is_positional = true;

  return *this;
}

template<typename T>
std::optional<T> Argument<T>::GetValue() const {

  if (auto single_value = std::get_if<SingleDataType<T>>(&value_)) {
    if (single_value->value != nullptr)
      return *single_value->value;
    else if (default_.has_value())
      return default_.value();

  }

  return std::optional<T>{};
}

template<typename T>
std::optional<T> Argument<T>::GetValue(int index) const {

  if (auto multi_value = std::get_if<MultiDataType<T> >(&value_)) {
    if (multi_value->value != nullptr && multi_value->value->size() > index)
      return multi_value->value->at(index);
    else if (default_.has_value())
      return default_.value();
  }

}

template<typename T>
void Argument<T>::AddValue(const T& value) {

  if (SingleDataType<T>* single_value = std::get_if<SingleDataType<T> >(&value_)) {

    if (single_value->value == nullptr)
      single_value->value = new T{value};
    else
      *single_value->value = T{value};

  } else if (MultiDataType<T>* multi_value = std::get_if<MultiDataType<T> >(&value_)) {
    if (multi_value->value == nullptr)
      multi_value->value = new std::vector<T>{};

    multi_value->value->push_back(value);
  }

}

template<typename T>
Argument<T> &Argument<T>::StoreValue(T& store_value) {
  std::get<SingleDataType<T>>(value_).value = &store_value;

  return *this;
}

template<typename T>
Argument<T> &Argument<T>::StoreValues(std::vector<T>& store_value) {
  std::get<MultiDataType<T>>(value_).value = &store_value;

  return *this;
}

template<typename T>
ArgStorageType Argument<T>::GetArgumentStoreType() const {

  if (std::get_if<SingleDataType<T> >(&value_))
    return ArgStorageType::Single;

  if (std::get_if<MultiDataType<T> >(&value_))
    return ArgStorageType::Multi;

}

template<typename T>
bool Argument<T>::DataIsCorrect() const {
  ArgStorageType arg_storage_type = GetArgumentStoreType();

  switch (arg_storage_type) {
    case ArgStorageType::Single: {
      if (!GetValue().has_value())
        return false;
      break;
    }
    case ArgStorageType::Multi:
      MultiDataType<T> data = std::get<MultiDataType<T>>(value_);
      if (data.value != nullptr && !default_.has_value()) {
        if (data.value->empty())
          return false;

        if (data.min_count.has_value() && data.value->size() < data.min_count.value())
          return false;
      } else
      if (!default_.has_value() && data.min_count.has_value())
        return false;

      break;
  }

  return true;
}

template<typename T>
bool Argument<T>::IsPositional() const {
  return is_positional;
}

template<typename T>
void Argument<T>::SetDescription(const char* desc) {
  description = desc;
}

template<typename T>
void Argument<T>::SetActive(bool active_status) {
  active_ = active_status;
}

template<typename T>
bool Argument<T>::IsActive() const {
  return active_;
}

template<typename T>
std::optional<T> Argument<T>::GetDefault() const {
  return default_;
}

template<typename T>
std::optional<size_t> Argument<T>::GetMinCount() const {
  return std::get<MultiDataType<T>>(value_).min_count;
}
