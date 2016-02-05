#ifndef FRUITCANDY_CORE_EVENT_H_
#define FRUITCANDY_CORE_EVENT_H_

#include <memory>
#include <string>

#include "value.h"
#include "../util/log.h"

class Event {
public:
  Event(const std::string &type)
      : params_(std::vector<std::unique_ptr<Value>>()) {
    type_ = type;
  }

  template<typename ... Args>
  Event(const std::string &type, Args ... args) : Event(type) {
    append_params(std::forward<Args>(args) ...);
  }

  virtual ~Event() = default;

public:
  template <typename T>
  void append_params(const T &t) {
    params_.append(t);
  }

  template <typename T, typename... Args>
  void append_params(T first, Args ... args) {
    params_.append(first);
    append_params(std::forward<Args>(args) ...);
  }

  template<typename ... Args>
  static std::unique_ptr<Event> create(Args  ... args) {
    return std::unique_ptr<Event>(new Event(std::forward<Args>(args) ...));
  }

public:
  std::string type_;
  Value params_;
};

#endif /* FRUITCANDY_CORE_EVENT_H_ */
