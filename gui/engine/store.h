#ifndef STORE_H_
#define STORE_H_

#include <map>
#include <memory>
#include <fstream>
#include "log.h"
#include "object.h"

class Store {
public:
  Store() = default;
  virtual ~Store() = default;

public:
  template <typename T>
  T* create_object() {
    // The objects are sorted by ID.
    // UINT_MAX should be enough to include all IDs.
    unsigned int id = objects_.size() == 0 ? 0 : objects_.rbegin()->first + 1;
    return create_object<T>(id);
  }

  template <typename T>
  T* create_object(unsigned int id) {
    objects_[id] = std::move(T::create());
    return get_object<T>(id);
  }


  template <typename T>
  T* get_object(unsigned int id) {
    if (objects_.find(id) == objects_.end())
      return nullptr;
    else
      return static_cast<T*>(objects_.at(id).get());
  }

public:
  std::map<unsigned int, std::unique_ptr<Object>> objects_;
};

#endif /* STORE_H_ */
