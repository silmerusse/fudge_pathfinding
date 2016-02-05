#ifndef FRUITCANDY_CORE_ASSET_H_
#define FRUITCANDY_CORE_ASSET_H_

#include "item.h"

class Asset : public Item {
public:
  Asset() = default;
  virtual ~Asset() = default;

public:
  std::string name_;
  std::string path_;
};

#endif /* FRUITCANDY_CORE_ASSET_H_ */
