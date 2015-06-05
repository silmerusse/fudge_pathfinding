#ifndef STORE_H_
#define STORE_H_

#include <map>
#include <memory>
#include <fstream>
#include <jsoncpp/json/json.h>
#include "log.h"
#include "loadable.h"
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
  void load_object(const Json::Value& v) {
    T* p = create_object<T>(v["id"].asUInt());
    p->load(v);
  }

  template <typename T>
  void load_objects(const std::string& path) {
    Json::Value root;
    Json::Reader reader;

    std::ifstream ifs(path);
    if (ifs.good()) {
      DEBUG("File opened: %s", path.c_str());
        if (reader.parse(ifs, root)) {
          DEBUG("Parsing JSON");
          for(Json::ValueIterator it = root.begin() ; it != root.end() ; ++it ) {
            Json::Value c = *it;
            DEBUG("Parsing object:\n%s", c.toStyledString().c_str());
            load_object<T>(c);
          }
        } else {
          ERROR("Failed to parse JSON.");
        }

    } else {
      ERROR("Couldn't open file.");
    }
    INFO("Store loaded: %s", path.c_str());
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
