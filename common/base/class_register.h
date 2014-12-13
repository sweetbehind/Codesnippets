
#ifndef CLASS_REGISTER_H_
#define CLASS_REGISTER_H_

#define PP_JOIN(X, Y) PP_DO_JOIN(X, Y)
#define PP_DO_JOIN(X, Y) PP_DO_JOIN2(X, Y)
#define PP_DO_JOIN2(X, Y) X##Y

#include <map>
#include <vector>
#include <string>

/*
   mapper.h (the interface definition):
   #include "common/base/class_register.h"
   class Mapper {
   };

   CLASS_REGISTER_DEFINE_REGISTRY(mapper_register, Mapper);

   #define REGISTER_MAPPER(mapper_name) \
       CLASS_REGISTER_OBJECT_CREATOR( \
           mapper_register, Mapper, #mapper_name, mapper_name) \

   #define CREATE_MAPPER(mapper_name_as_string) \
       CLASS_REGISTER_CREATE_OBJECT(mapper_register, mapper_name_as_string)

   hello_mapper.cc (an implementation of Mapper):
   #include "mapper.h"
   class HelloMapper : public Mapper {
   };
   REGISTER_MAPPER(HelloMapper);

   mapper_user.cc (the final user of all registered mappers):
   #include "mapper.h"
   Mapper* mapper = CREATE_MAPPER("HelloMapper");
*/

// ClassRegistry manage (name -> creator) mapping.
// One base class may have multiple registry instance, distinguished by the
// register_name.
template <typename BaseClassName>
class ClassRegistry {
 public:
  typedef BaseClassName* (*Creator)();
 
 private:
  typedef std::map<std::string, Creator> ClassMap;

 public:
  ClassRegistry() {}
  ~ClassRegistry() {}

  void AddCreator(const std::string& entry_name, Creator creator) {
    typename ClassMap::iterator it = creator_map_.find(entry_name);
    if (it != creator_map_.end()) {
      fprintf(stderr,
              "ClassRegister: class %s already registered.",
              entry_name.c_str());
      abort();
    }
    creator_map_.insert(make_pair(entry_name, creator));
    creator_names_.push_back(entry_name);
  }

  BaseClassName* CreateObject(const std::string& entry_name) const {
    typename ClassMap::const_iterator it = creator_map_.find(entry_name);
    if (it == creator_map_.end()) {
      return NULL;
    }
    return (it->second)();
  }
 
 private:
  std::vector<std::string> creator_names_;
  ClassMap creator_map_;
};

// Get the registry singleton instance for a given register_name
template <typename RegistryTag>
ClassRegistry<typename RegistryTag::BaseClass>& GetRegistry() {
    static ClassRegistry<typename RegistryTag::BaseClass> registry;
    return registry;
}

// CLASS_REGISTER_DEFINE_REGISTRY Make a unique type for a given register_name.
// This class is the base of the generated unique type
template <typename BaseClassName>
struct ClassRegistryTagBase {
    typedef BaseClassName BaseClass;
};

// All class can share the same creator as a function template
template <typename BaseClassName, typename SubClassName>
BaseClassName* ClassRegistry_NewObject() {
    return new SubClassName();
}

// Used to register a given class into given registry
template <typename RegistryTag>
class ClassRegisterer {
    typedef typename RegistryTag::BaseClass BaseClassName;
public:
    ClassRegisterer(
        const std::string& entry_name,
        typename ClassRegistry<BaseClassName>::Creator creator) {
      GetRegistry<RegistryTag>().AddCreator(entry_name, creator);
    }
    ~ClassRegisterer() {}
};

// Define a registry for a base class.
//
// The first parameter, register_name, should be unique globally.
// Mulitiple registry can be defined for one base class with different
// register_name.
//
// This macro should be used in the same namespace as base_class_name.
#define CLASS_REGISTER_DEFINE_REGISTRY(register_name, base_class) \
    struct register_name##RegistryTag: \
        public ::ClassRegistryTagBase<base_class> {};

// These macros should be used in the same namespace as class_name, and
// class_name should not be namespace prefixed.
//
// But namespace prefix is required for register_name and base_class_name if
// they are defined in different namespace, for example, ::common::File
//
#define CLASS_REGISTER_OBJECT_CREATOR(register_name, \
                                      base_class, \
                                      entry_name, \
                                      sub_class) \
    static ClassRegisterer<register_name##RegistryTag> \
        PP_JOIN(g_object_creator_register_##sub_class, __LINE__)( \
            entry_name, \
            &ClassRegistry_NewObject<base_class, sub_class>)

// Create object from registry by name.
// Namespace prefix is required for register_name if it is defined in different
// namespace
#define CLASS_REGISTER_CREATE_OBJECT(register_name, entry_name_as_string) \
    GetRegistry<register_name##RegistryTag>().CreateObject(entry_name_as_string)

#endif
