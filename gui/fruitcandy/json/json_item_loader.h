#ifndef FRUITCANDY_JSON_JSON_ITEM_LOADER_H_
#define FRUITCANDY_JSON_JSON_ITEM_LOADER_H_

#include <fstream>
#include "../util/log.h"
#include "../engine/repository.h"
#include "document.h"

class JSONItemLoader {
public:
  template <typename T>
  T* load_item(Repository *repo, const std::string &tag, Value *value) {
    T *p = repo->create_item<T>(value->get_int("id"), tag);
    p->load(*value);
    return p;
  }

  template <typename T>
  bool load_items(Repository *repo,
                  const std::string &tag,
                  const std::string &path) {
    Document doc;

    std::ifstream ifs(path);
    if (ifs.good()) {
      DEBUG("File opened: %s", path.c_str());
        if (doc.parse(ifs)) {
          DEBUG("Parsing JSON");
          for(auto &v : doc.root_->as_list()) {
            load_item<T>(repo, tag, v.get());
          }
        } else {
          ERROR("Failed to parse JSON.");
          return false;
        }
    } else {
      ERROR("Couldn't open file.");
      return false;
    }
    INFO("Store loaded: %s", path.c_str());
    return true;
  }

};

#endif /* FRUITCANDY_JSON_JSON_ITEM_LOADER_H_ */
