#ifndef LOADABLE_H_
#define LOADABLE_H_

#include <jsoncpp/json/json.h>

class Loadable {
public:
  Loadable() = default;
  virtual ~Loadable() = default;

public:
  virtual void load(const Json::Value& v) {};

};

#endif /* LOADABLE_H_ */
