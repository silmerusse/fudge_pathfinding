#ifndef EVENT_H_
#define EVENT_H_

#include <memory>
#include <string>
#include "value.h"
#include "log.h"

class Event {
public:
  Event(const std::string &type) {
    type_ = type;
  }

  template<typename ... Args>
  Event(const std::string &type, Args ... args) {
    type_ = type;
    append_params(std::forward<Args>(args) ...);
  }

  virtual ~Event() = default;

public:
  template <typename T>
  void append_params (const T &t) {
    params_.append(ValueFactory::create(t));
  }

  template <typename T, typename... Args>
  void append_params (T first, Args ... args) {
    params_.append(ValueFactory::create(first));
    append_params(std::forward<Args>(args) ...);
  }

  template<typename ... Args>
  static std::unique_ptr<Event> create (Args  ... args) {
    return std::unique_ptr<Event>(new Event(std::forward<Args>(args) ...));
  }

public:
  std::string type_;
  ArrayValue params_;
};

#endif /* EVENT_H_ */
