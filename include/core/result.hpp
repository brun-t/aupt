#pragma once

#include <concepts>
#include <functional>
#include <optional>
#include <source_location>
#include <stdexcept>
#include <string>

namespace core {

template <typename E>
concept ConvertibleToString = std::convertible_to<E, std::string>;

struct empty {};

template <typename T>
using storage_t = std::conditional_t<std::is_void_v<T>, empty, T>;

template <typename T = void, typename E = std::string>
requires ConvertibleToString<E> class Result {
public:
  using Value = storage_t<T>;
  using Err = std::function<E()>;

  // Success constructor
  template <typename U = T>
  Result(const Value &value) requires(!std::is_same_v<U, void>)
      : value(value), err([] { return E(); }) {
  }

  // Failure constructor with lambda
  Result(const Err &err)
      : value(std::nullopt), err(err) {
  }

  // Failure constructor with string-like error
  Result(const E &errVal, const std::source_location loc = std::source_location::current()) {
    this->value = std::nullopt;
    this->err = [errVal, loc]() {
      if constexpr (std::is_same_v<E, std::string>)
        return errVal + " -> (File: " + loc.file_name() + " Line: " + std::to_string(loc.line()) + ")";
      else
        return errVal;
    };
  }

  // Success for void
  Result() requires std::is_same_v<T, void>
      : value(empty{}), err([] { return E(); }) {
  }

  operator bool() const noexcept {
    return value.has_value();
  }

  T unwrap() const requires(!std::is_same_v<T, void>) {
    if (*this)
      return *value;
    throw std::runtime_error(this->error_string());
  }

  E error() const noexcept {
    return err ? err() : E();
  }

  std::string error_string() const noexcept {
    if (!err)
      return {};
    return std::string(err());
  }

  template <typename F>
  Result<T> then(F &&callback) const
      noexcept(std::is_nothrow_invocable_v<F> || std::is_nothrow_invocable_v<F, const T &>) {
    if constexpr (std::is_same_v<T, void>) {
      if (*this)
        callback();
      return *this;
    } else {
      if (*this)
        callback(*value);
      return *this;
    }
  }

  template <typename OnOk, typename OnErr>
      requires(std::invocable<OnOk> || std::invocable<OnOk, const T &>) &&
      std::invocable<OnErr, E> auto match(OnOk ok, OnErr errHandler) const {
    if (*this) {
      if constexpr (std::is_same_v<T, void>)
        return ok();
      else
        return ok(*value);
    } else {
      return errHandler(this->error());
    }
  }

private:
  std::optional<Value> value;
  Err err;
};

// Helpers

inline Result<void> Ok() noexcept {
  return Result<void>();
}

template <typename T>
inline Result<T> OkWithValue(const T &value) noexcept {
  return Result<T>(value);
}

template <typename T = void>
inline Result<T> Err(const std::string &message, const std::source_location loc = std::source_location::current()) noexcept {
  return Result<T>(message, loc);
}

} // namespace core
