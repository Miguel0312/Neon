#include "utils/objectFactory.h"
#include <iostream>

namespace Neon {
std::map<std::string, ObjectFactory::Constructor>
    *ObjectFactory::m_constructors = nullptr;

NeonObject *ObjectFactory::instantiate(const std::string &name,
                                       const toml::table *table) {
  if (m_constructors == nullptr) {
    std::cout << "ERROR: no class has been registered yet" << std::endl;
    return nullptr;
  }

  auto it = m_constructors->find(name);
  if (it == m_constructors->end()) {
    std::cout << "ERROR: no class has been registered with the name " << name
              << std::endl;
    return nullptr;
  }

  return it->second(table);
}

void ObjectFactory::registerClass(const std::string &name,
                                  const Constructor &constructor) {
  if (m_constructors == nullptr) {
    m_constructors = new std::map<std::string, Constructor>();
  }

  (*m_constructors)[name] = constructor;
}
} // namespace Neon