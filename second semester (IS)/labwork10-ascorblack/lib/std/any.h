#pragma once

class Any {
 public:
  template<typename T>
  explicit Any(const T& value) : ptr(new Holder<T>(value)) {}

  explicit Any() : ptr(nullptr) {}

  Any(const Any& other) : ptr(nullptr) {
    if (other.ptr) {
      ptr = other.ptr->clone();
    }
  }

  ~Any() {
    if (ptr)
      delete ptr;
  }

  explicit operator bool() const {
    return ptr != nullptr;
  }

  Any& operator=(const Any& other) {
    if (this != &other) {
      if (ptr)
        delete ptr;

      if (other.ptr) {
        ptr = other.ptr->clone();
      } else {
        ptr = nullptr;
      }
    }
    return *this;
  }

  template<typename T>
  T get() const {
    if (!ptr)
      throw std::runtime_error("Any: Trying to get value from null pointer");

    if (typeid(Holder<T>) != typeid(*ptr))
      throw std::runtime_error("Any: Type mismatch");

    return static_cast<Holder<T>*>(ptr)->value;
  }

 private:
  class BaseHolder {
   public:
    virtual ~BaseHolder() = default;
    virtual BaseHolder* clone() const = 0;
  };

  template<typename T>
  class Holder : public BaseHolder {
   public:
    explicit Holder(const T& val) : value(val) {}

    T value;

    BaseHolder* clone() const override {
      return new Holder<T>(value);
    }
  };

  BaseHolder* ptr;
};