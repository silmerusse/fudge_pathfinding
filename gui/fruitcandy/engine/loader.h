#ifndef FRUITCANDY_ENGINE_LOADER_H_
#define FRUITCANDY_ENGINE_LOADER_H_

#include "value.h"

class Item;

class Loader {
public:
  Loader() = default;
  virtual ~Loader() = default;

public:
  virtual void load(Item *item, Value &v) = 0;
};

#endif /* FRUITCANDY_ENGINE_LOADER_H_ */
