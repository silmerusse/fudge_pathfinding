#ifndef FRUITCANDY_CORE_ITEM_H_
#define FRUITCANDY_CORE_ITEM_H_

#include "loader.h"

class Item {
public:
  Item() = default;
  virtual ~Item() = default;

public:
  virtual void load(Value &v) {
    if (loader_)
      loader_->load(this, v);
    else
      ERROR("No loader to load object.");
  }

  // Represent as a string.
  virtual const std::string to_string() const {
    std::ostringstream ss;
    ss << "id:" << id_ << " | "
          "tag:" << tag_ << " | ";
    return ss.str();
  }

public:
  int id_ = -1;
  std::string tag_ = "";
  Loader *loader_ = nullptr; // This should be a singleton.
};

#endif /* FRUITCANDY_CORE_ITEM_H_ */
