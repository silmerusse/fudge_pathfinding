#ifndef FRUITCANDY_ENGINE_VALUE_H_
#define FRUITCANDY_ENGINE_VALUE_H_

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <boost/any.hpp>
#include "../util/log.h"

class Value {
protected:
  boost::any v_;

public:
  Value() : v_(boost::any()) {}
  Value(const boost::any &v) : v_(v) {}
  Value(std::vector<std::unique_ptr<Value>> list) {
    list_.reset(new std::vector<std::unique_ptr<Value>>(std::move(list)));
  }
  Value(std::map<std::string, std::unique_ptr<Value>> dict) {
    dict_.reset(new std::map<std::string, std::unique_ptr<Value>>(std::move(dict)));
  }
  virtual ~Value() = default;

public:
  virtual Value* get(int index) {
    return as_list().at(index).get();
  }

  virtual Value* get(const std::string &key) {
    return as_dict().at(key).get();
  }

  virtual int get_int(int index) {
    return as_list().at(index)->as_int();
  }

  virtual int get_int(const std::string &key) {
    return as_dict().at(key)->as_int();
  }

  virtual double get_double(int index) {
    return as_list().at(index)->as_double();
  }

  virtual double get_double(const std::string &key) {
    return as_dict().at(key)->as_double();
  }

  virtual std::string get_string(int index) {
    return as_list().at(index)->as_string();
  }

  virtual std::string get_string(const std::string &key) {
    return as_dict().at(key)->as_string();
  }

  virtual int as_int() const {
    return boost::any_cast<int>(v_);
  }

  virtual double as_double() const {
    return boost::any_cast<double>(v_);
  }

  virtual const std::string as_string() const {
    return boost::any_cast<const std::string>(v_);
  }

  virtual void append(std::unique_ptr<Value> value) {
    as_list().push_back(std::move(value));
  }

  virtual void append(const boost::any &v) {
    std::unique_ptr<Value> value(new Value(v));
    append(std::move(value));
  }

  virtual void insert(std::pair<std::string, std::unique_ptr<Value>> pair) {
    as_dict().emplace(std::move(pair));
  }

  virtual bool has(const std::string &key) const {
    if (dict_)
      return dict_->find(key) != dict_->end();
    else throw; // Not a dict.
  }

  virtual int length() const {
    if (list_)
      return list_->size();
    else if (dict_)
        return dict_->size();
    else
      throw; // Not a list or dict.
  }

  virtual bool empty() const {
    if (list_)
      return list_->empty();
    else if (dict_)
      return dict_->empty();
    else throw; // Not a list or dict.
  }

  virtual std::vector<std::unique_ptr<Value>>& as_list() {
    if (list_)
      return *list_;
    else throw; // Not a list.
  }

  virtual std::map<std::string, std::unique_ptr<Value>>& as_dict() {
    if (dict_)
      return *dict_;
    else throw; // Not a dict.
  }

protected:
  std::unique_ptr<std::vector<std::unique_ptr<Value>>> list_ = nullptr;
  std::unique_ptr<std::map<std::string, std::unique_ptr<Value>>> dict_ = nullptr;

public:
  static std::unique_ptr<Value> create(const boost::any &v) {
    return std::move(std::unique_ptr<Value>(new Value(v)));
  }
};

#endif /* FRUITCANDY_ENGINE_VALUE_H_ */
