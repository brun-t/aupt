#pragma once

#include <functional>
#include <optional>
#include <source_location>
#include <stdexcept>
#include <string>
#include <variant>


namespace core {

// Used as stand-in when T is void
struct empty {};

template <typename T>
struct StorageType {
  using type = T;
};

template <>
struct StorageType<void> {
  using type = empty;
};

template <typename T>
using storage_t = typename StorageType<T>::type;

template <typename T = void>
class Result {
public:
  using Err = std::function<std::string()>;
  using Value = core::storage_t<T>; // T or empty

  template <typename U = T>
  Result(const Value &value) requires !(std::is_same_v<U, void>)
      : value(value), err([]() { return ""; }) {
  }

  Result(const Err &err)
      : value(std::nullopt), err(err) {
  }

  Result(const std::string &text, const std::source_location loc = std::source_location::current()) {
    this->value = std::nullopt;
    this->err = [text, loc]() {
      return text + " -> (File: " + loc.file_name() + " Line: " + std::to_string(loc.line()) + ")";
    };
  }

  Result() requires std::is_same_v<T, void>
      : value(core::empty{}), err([] { return ""; }) {
  }

  operator bool() const {
    return value.has_value();
  }

  T unwrap() const requires(!std::is_same_v<T, void>) {
    if (*this)
      return *value;
    throw std::runtime_error(this->error());
  }

  std::string error() const {
    return err ? err() : "";
  }

  template <typename F>
  auto then(F &&callback) const -> Result<T> {
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
      auto match(OnOk ok, OnErr errHandler) const -> decltype(auto) requires(std::invocable<OnOk> || std::invocable<OnOk, const T &>) &&
      std::invocable<OnErr, std::string> {
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

inline Result<void> Ok() {
  return Result<void>();
}

template <typename T>
inline Result<T> OkWithValue(const T &value) {
  return Result<T>(value);
}

template <typename T = void>
inline Result<T> Err(const std::string &message, const std::source_location loc = std::source_location::current()) {
  return Result<T>(message, loc);
}

} // namespace core