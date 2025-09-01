#include "generation.hpp"

#include <sstream>

Generator::Generator(Node::Exit root)
  : m_Root(std::move(root))
{

}

std::string Generator::generate() const 
{
  std::stringstream output;
  output << "global _start\n_start:\n";

  output << "    mov eax, " << m_Root.expr.int_lit.value.value() << "\n";
  output << "    ret\n";

  return output.str();
}
