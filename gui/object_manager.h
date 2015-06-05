#ifndef OBJECT_STORE_H_
#define OBJECT_STORE_H_

#include <map>
#include <memory>
#include <tuple>
#include "store.h"

class ObjectManager {
public:
  ObjectManager() = default;
  virtual ~ObjectManager() = default;

public:
  template <typename T>
  void load_store(const std::string &name, const std::string &path) {
    std::unique_ptr<Store> store(new Store());
    store->load_objects<T>(path);
    objects_[name] = std::move(store);
  }

  void add_store(const std::string &name) {
    std::unique_ptr<Store> store(new Store());
    objects_[name] = std::move(store);
  }

  Store* get_store(const std::string &name) {
    return objects_.at(name).get();
  }

  template <typename T>
  void traverse_store(const std::string &name, std::function<void(T*)> f) {
    std::unique_ptr<Store> &store = objects_.at(name);
    for (auto &object : store->objects_) {
      f(static_cast<T*>(object.second.get()));
    }
  }

  void traverse_all(std::function<void(Object*)> f) {
    for (auto &p: objects_) {
      auto &store = p.second;
      for (auto &object : store->objects_) {
        f(object.second.get());
      }
    }
  }

  template <typename T>
  T* create_object(const std::string &name) {
    return get_store(name)->create_object<T>();
  }

  template <typename T>
  T* get_object(const std::string &name, unsigned int id) {
    return get_store(name)->get_object<T>(id);
  }

protected:
  std::map<std::string, std::unique_ptr<Store>> objects_;
};

#endif /* OBJECT_STORE_H_ */
