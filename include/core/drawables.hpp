#include "renderer.hpp"
#include <glm/glm.hpp>
#include <iostream>

namespace core {
class Mesh : public core::Drawable {
public:
  Mesh(sf::PrimitiveType type = sf::PrimitiveType::Triangles)
      : vertices(type), position(glm::vec2{0.f, 0.f}), scale(glm::vec2{1.f, 1.f}), rotation(0) {
  }

  void AddVertex(const glm::vec2 &pos, const sf::Color &color = sf::Color::White) {
    this->vertices.append(sf::Vertex(sf::Vector2f(pos.x, pos.y), color));
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

  core::Result<std::pair<const sf::Drawable *, sf::Transform>> Draw(float, sf::RenderTexture &) override {
    // Create a transformed drawable object
    transformed = this->vertices; // copy

    sf::Transform transform;
    transform.translate(sf::Vector2(position.x, position.y));
    transform.rotate(sf::degrees(rotation));

    transform.scale(sf::Vector2f(scale.x, scale.y), sf::Vector2f(0.f, 0.f));

    return core::OkWithValue(std::pair{static_cast<const sf::Drawable *>(&transformed), transform});
  }

private:
  sf::VertexArray vertices;
  sf::VertexArray transformed; // used to draw after transformation
  glm::vec2 position;
  glm::vec2 scale;
  float rotation;
};
} // namespace core
