#ifndef EVENT_H_
#define EVENT_H_

#include <memory>
#include <string>
#include <jsoncpp/json/json.h>
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
  void unpack(const std::string& s) {
    Json::Value root;
    Json::Reader reader;

    if (reader.parse(s, root)) {
      type_ = root["type"].asString();
      params_ = root["params"];
    } else {
      ERROR("parsing event failed.");
    }
  }

  template <typename T>
  void append_params (const T &t) {
    params_.append(Json::Value(t));
  }

  template <typename T, typename... Args>
  void append_params (T first, Args ... args) {
    params_.append(Json::Value(first));
    append_params(std::forward<Args>(args) ...);
  }


  template<typename ... Args>
  static std::unique_ptr<Event> create (Args  ... args) {
    return std::unique_ptr<Event>(new Event(std::forward<Args>(args) ...));
  }

public:
  std::string type_;
  Json::Value params_;
};

#endif /* EVENT_H_ */
