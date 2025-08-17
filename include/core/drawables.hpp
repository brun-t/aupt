#pragma once

#ifndef _NOMINMAX
#define _NOMINMAX
#endif

#include "embeds.hpp"
#include "renderer.hpp"
#include <filesystem>
#include <glm/vec2.hpp>
#include <iostream>
#include <memory>

namespace core {
using Primitives = sf::PrimitiveType;
class Mesh : public core::Drawable {
public:
  Mesh(Primitives type = Primitives::Triangles)
      : vertices(type), position(glm::vec2{0.f, 0.f}), scale(glm::vec2{1.f, 1.f}), rotation(0) {
  }

  Result<void> AddVertex(const glm::vec2 &pos, const std::string &color = "#fff", const std::source_location loc = std::source_location::current()) {
    Result<sf::Color> hex = parseHexColor(color);
    try {
      this->vertices.append(sf::Vertex(sf::Vector2f(pos.x, pos.y), hex.unwrap()));
    } catch (std::exception e) {
      return Err(e.what(), loc);
    }
    return Ok();
  }

  void SetPosition(const glm::vec2 &pos) override {
    this->position = pos;
  }

  void SetRotation(float rot) override {
    this->rotation = rot;
  }

  void SetScale(const glm::vec2 &scl) override {
    this->scale = scl;
  }

  glm::vec2 GetPosition() override {
    return this->position;
  }

  float GetRotation() override {
    return this->rotation;
  }

  glm::vec2 GetScale() override {
    return this->scale;
  };

  Transform GetTransform() override {
    return Transform{
        .position = GetPosition(),
        .scale = GetScale(),
        .rotation = GetRotation(),
    };
  }

  core::Result<std::pair<const sf::Drawable *, sf::Transform>> Draw(float, sf::RenderTexture &) override {
    sf::Transform transform;
    transform.translate(sf::Vector2(position.x, position.y));
    transform.rotate(sf::degrees(rotation));

    transform.scale(sf::Vector2f(scale.x, scale.y), sf::Vector2f(0.f, 0.f));

    return core::OkWithValue(std::pair{static_cast<const sf::Drawable *>(&vertices), transform});
  }

private:
  sf::VertexArray vertices;
  glm::vec2 position;
  glm::vec2 scale;
  float rotation;
};

class Sprite : public core::Drawable {
public:
  Sprite() = default; // allow default construction

  Sprite(const std::filesystem::path &path, const std::filesystem::path &cwd = std::filesystem::current_path()) {
    if (!texture.loadFromFile(cwd / path)) {
      throw std::runtime_error("Failed to load texture");
    }
    sprite = std::make_unique<sf::Sprite>(texture);
  }

  void SetPosition(const glm::vec2 &pos) override {
    position = pos;
  }
  void SetRotation(float rot) override {
    rotation = rot;
  }
  void SetScale(const glm::vec2 &scl) override {
    scale = scl;
  }

  glm::vec2 GetPosition() override {
    return this->position;
  }

  float GetRotation() override {
    return this->rotation;
  }

  glm::vec2 GetScale() override {
    return this->scale;
  }

  Transform GetTransform() override {
    return Transform{
        .position = GetPosition(),
        .scale = GetScale(),
        .rotation = GetRotation(),
    };
  }

  core::Result<std::pair<const sf::Drawable *, sf::Transform>> Draw(float, sf::RenderTexture &) override {
    sf::Transform transform;
    transform.translate(sf::Vector2f(position.x, position.y));
    transform.rotate(sf::degrees(rotation));
    transform.scale(sf::Vector2f(scale.x, scale.y));
    return core::OkWithValue(std::pair{static_cast<const sf::Drawable *>(&(*sprite)), transform});
  }

private:
  sf::Texture texture;
  std::unique_ptr<sf::Sprite> sprite; // There isn't another way of solve this
  glm::vec2 position{0.f, 0.f};
  glm::vec2 scale{1.f, 1.f};
  float rotation = 0.f;
};

class Circle : public core::Drawable {
public:
  Circle() = default; // allow default construction

  Circle(float radius) {
    this->circle = sf::CircleShape(radius);
  }

  Result<void> ChangeColor(const std::string &color, const std::source_location loc = std::source_location::current()) {
    return parseHexColor(color).match(
        [this](sf::Color color) -> Result<void> {
          circle.setFillColor(color);
          return Ok();
        },
        [loc](auto err) -> Result<void> {
          return Err(err, loc);
        });
  }

  void SetPosition(const glm::vec2 &pos) override {
    position = pos;
  }

  void SetRotation(float rot) override {
    rotation = rot;
  }

  void SetScale(const glm::vec2 &scl) override {
    scale = scl;
  }

  glm::vec2 GetPosition() override {
    return this->position;
  }

  float GetRotation() override {
    return this->rotation;
  }

  glm::vec2 GetScale() override {
    return this->scale;
  }

  Transform GetTransform() override {
    return Transform{
        .position = GetPosition(),
        .scale = GetScale(),
        .rotation = GetRotation(),
    };
  }

  core::Result<std::pair<const sf::Drawable *, sf::Transform>> Draw(float, sf::RenderTexture &) override {
    sf::Transform transform;
    transform.translate(sf::Vector2f(position.x, position.y));
    transform.rotate(sf::degrees(rotation));
    transform.scale(sf::Vector2f(scale.x, scale.y));
    return core::OkWithValue(std::pair{static_cast<const sf::Drawable *>(&circle), transform});
  }

private:
  sf::CircleShape circle; // There isn't another way of solve this
  glm::vec2 position{0.f, 0.f};
  glm::vec2 scale{1.f, 1.f};
  float rotation = 0.f;
};

class Text : public core::Drawable {
public:
  Text() = default;

  Text(const std::string &text) {
    if (!font.openFromMemory(Roboto_font, Roboto_font_size)) {
      throw std::runtime_error("Failed to load embedded Roboto font");
    }
    this->text = std::make_unique<sf::Text>(font);
    this->text->setString(text);
  }

  void SetFontSize(ui size) {
    text->setCharacterSize(size);
  }

  Result<void> LoadFont(const std::filesystem::path &path, const std::filesystem::path &cwd = std::filesystem::current_path(), std::source_location loc = std::source_location::current()) {
    if (!this->font.openFromFile(cwd / path)) {
      return Err("Couldn't find file: " + (cwd / path).generic_string(), loc);
    }
    this->text->setFont(this->font); // just in case reassign it for it to update
    return Ok();
  }

  Result<void> SetColor(const std::string &hex, std::source_location loc = std::source_location::current()) {
    return parseHexColor(hex).match(
        [this](sf::Color color) -> Result<void> {
          this->text->setFillColor(color);
          return Ok();
        },
        [loc](auto err) -> Result<void> {
          return Err(err, loc);
        });
  }

  void SetPosition(const glm::vec2 &pos) override {
    position = pos;
  }

  void SetRotation(float rot) override {
    rotation = rot;
  }

  void SetScale(const glm::vec2 &scl) override {
    scale = scl;
  }

  glm::vec2 GetPosition() override {
    return this->position;
  }

  float GetRotation() override {
    return this->rotation;
  }

  glm::vec2 GetScale() override {
    return this->scale;
  }

  Transform GetTransform() override {
    return Transform{
        .position = GetPosition(),
        .scale = GetScale(),
        .rotation = GetRotation(),
    };
  }

  core::Result<std::pair<const sf::Drawable *, sf::Transform>> Draw(float, sf::RenderTexture &renderTexture) override {
    static bool baselineCaptured = false;
    static sf::Vector2u baselineWindowSize;
    static unsigned int baselineFontSize;

    sf::Vector2u currentSize = renderTexture.getSize();

    if (!baselineCaptured) {
      baselineWindowSize = currentSize; // capture first window size as baseline
      baselineFontSize = text->getCharacterSize(); // capture initial font size
      baselineCaptured = true;
    }

    // Calculate scale ratio based on first captured size
    float scaleRatio = static_cast<float>(currentSize.y) / baselineWindowSize.y;

    // Calculate new font size
    unsigned int newFontSize = static_cast<unsigned int>(baselineFontSize * scaleRatio);

    // Clamp font size
    newFontSize = std::clamp(newFontSize, 8u, 72u);

    // Only update font size if changed
    if (text->getCharacterSize() != newFontSize) {
      text->setCharacterSize(newFontSize);
    }

    // Build transform without scale (avoid bitmap zoom)
    sf::Transform transform;
    transform.translate(sf::Vector2f(position.x, position.y));
    transform.rotate(sf::degrees(rotation));
    transform.scale(sf::Vector2f(scale.x, scale.y));

    return core::OkWithValue(std::pair{static_cast<const sf::Drawable *>(&(*text)), transform});
  }

private:
  sf::Font font;
  std::unique_ptr<sf::Text> text;
  glm::vec2 position{0.f, 0.f};
  glm::vec2 scale{1.f, 1.f};
  float rotation = 0.f;
};

class Rect : public core::Drawable {
public:
  Rect(glm::vec2 &rect) {
    _rect = sf::RectangleShape(sf::Vector2f{rect.x, rect.y});
  }

  void SetPosition(const glm::vec2 &pos) override {
    this->position = pos;
  }

  void SetRotation(float rot) override {
    this->rotation = rot;
  }

  void SetScale(const glm::vec2 &scl) override {
    this->scale = scl;
  }

  glm::vec2 GetPosition() override {
    return this->position;
  }
  float GetRotation() override {
    return this->rotation;
  }
  glm::vec2 GetScale() override {
    return this->scale;
  }

  core::Result<std::pair<const sf::Drawable *, sf::Transform>> Draw(float, sf::RenderTexture &) override {
    sf::Transform transform;
    transform.translate(sf::Vector2(position.x, position.y));
    transform.rotate(sf::degrees(rotation));

    transform.scale(sf::Vector2f(scale.x, scale.y), sf::Vector2f(0.f, 0.f));

    return core::OkWithValue(std::pair{static_cast<const sf::Drawable *>(&_rect), transform});
  }

private:
  sf::RectangleShape _rect;
  glm::vec2 position{0.f, 0.f};
  glm::vec2 scale{1.f, 1.f};
  float rotation = 0;
};
} // namespace core