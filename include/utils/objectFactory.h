#ifndef NEON_OBJECT_FACTORY_H
#define NEON_OBJECT_FACTORY_H

#include "thirdparty/toml++/toml.h"
#include <functional>
namespace Neon {
class NeonObject {
protected:
  NeonObject() = default;

  ~NeonObject() = default;
};

class ObjectFactory {
public:
  typedef std::function<NeonObject *(const toml::table *table)> Constructor;

  static NeonObject *instantiate(const std::string &name,
                                 const toml::table *table);

  static void registerClass(const std::string &name,
                            const Constructor &constructor);

private:
  static std::map<std::string, Constructor> *m_constructors;
};

// Macro from the nori renderer (https://github.com/wjakob/nori)
#define NEON_REGISTER_CLASS(cls, name)                                         \
  cls *cls##_create(const toml::table *table) { return new cls(table); }       \
  static struct cls##_ {                                                       \
    cls##_() { ObjectFactory::registerClass(name, cls##_create); }             \
  } cls##__NORI_;
} // namespace Neon

#endif