#pragma once

#include "parser.hpp"

class Generator {
public:
  Generator(Node::Exit root);

  [[nodiscard]] std::string generate() const;

private:
  const Node::Exit m_Root;
};
