#pragma once

#include "result.hpp"
#include <SFML/Graphics/Color.hpp>
#include <cstdint>
#include <string>

namespace core {

inline Result<sf::Color> parseHexColor(const std::string &hex) {
  std::string clean;

  if (hex.empty())
    return Err<sf::Color>("Hex color is empty");

  if (hex[0] == '#')
    clean = hex.substr(1);
  else
    clean = hex;

  auto fromHex = [](const std::string &s) -> Result<uint8_t> {
    try {
      return OkWithValue(static_cast<uint8_t>(std::stoi(s, nullptr, 16)));
    } catch (...) {
      return Err<uint8_t>("Invalid hex value: '" + s + "'");
    }
  };

  if (clean.length() == 6) {
    auto r = fromHex(clean.substr(0, 2));
    auto g = fromHex(clean.substr(2, 2));
    auto b = fromHex(clean.substr(4, 2));

    if (!r)
      return Err<sf::Color>(r.error());
    if (!g)
      return Err<sf::Color>(g.error());
    if (!b)
      return Err<sf::Color>(b.error());

    return OkWithValue(sf::Color(r.unwrap(), g.unwrap(), b.unwrap()));
  }

  if (clean.length() == 3) {
    auto expand = [](char c) -> std::string {
      return std::string(2, c);
    };

    auto r = fromHex(expand(clean[0]));
    auto g = fromHex(expand(clean[1]));
    auto b = fromHex(expand(clean[2]));

    if (!r)
      return Err<sf::Color>(r.error());
    if (!g)
      return Err<sf::Color>(g.error());
    if (!b)
      return Err<sf::Color>(b.error());

    try {
      return OkWithValue(sf::Color(r.unwrap(), g.unwrap(), b.unwrap()));
    } catch (std::exception e) {
      return Err<sf::Color>(std::string(e.what()));
    }
  }

  return Err<sf::Color>("Invalid hex color format: '" + hex + "'");
}

} // namespace core
