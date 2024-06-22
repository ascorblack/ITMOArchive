#pragma once

template<typename T>
constexpr T&& forward(T& arg) noexcept {
  return static_cast<T&&>(arg);
}

template<typename T>
constexpr T&& forward(T&& arg) noexcept {
  return static_cast<T&&>(arg);
}

