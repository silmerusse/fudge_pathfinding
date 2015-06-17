#ifndef VALUE_H_
#define VALUE_H_

#include <exception>
#include <string>
#include <vector>
#include <map>
#include <memory>

class BadCast : public std::exception {
public:
  const char* what() const throw () {
    return "Bad cast.";
  }
};

class Value {
public:
  Value() = default;
  virtual ~Value() = default;

public:
  // Type cast
  virtual int as_int() const {
    throw BadCast();
  }

  virtual int as_uint() const {
    throw BadCast();
  }

  virtual double as_double() const {
    throw BadCast();
  }

  virtual const std::string as_string() const {
    throw BadCast();
  }

  virtual std::vector<std::unique_ptr<Value>>& as_array() {
    throw BadCast();
  }

  virtual std::map<std::string, std::unique_ptr<Value>>& as_object() {
    throw BadCast();
  }

  // For object type
  virtual const std::unique_ptr<Value>&
  at(const std::string &key) const {
    throw BadCast();
  }

  virtual std::unique_ptr<Value>&
  at(const std::string &key) {
    throw BadCast();
  }

  virtual const std::unique_ptr<Value>&
  operator [] (const std::string &key) const {
    throw BadCast();
  }

  // For array type
  virtual const std::unique_ptr<Value>&
  operator [] (unsigned int i) const {
    throw BadCast();
  }

  virtual std::unique_ptr<Value>&
  operator [] (unsigned int i) {
    throw BadCast();
  }

  virtual void append(std::unique_ptr<Value> v) {
    throw BadCast();
  }
};

class NumValue: public Value {
  union Num {
    int n_;
    double d_;
    Num(int n = 0): n_(n) {};
    Num(double d): d_(d) {};
  } num_;

public:
  NumValue(int n) : num_(n) {};
  NumValue(double d) : num_(d) {};

public:
  int as_int() const override {
    return num_.n_;
  }

  int as_uint() const override {
    return static_cast<unsigned int>(num_.n_);
  }

  double as_double() const override {
    return num_.d_;
  }
};

class StringValue: public Value {
public:
  StringValue(std::string s) : s_(std::move(s)) {};

public:
  const std::string as_string() const override {
    return s_;
  }

private:
  std::string s_;
};

class ObjectValue: public Value {
public:
  ObjectValue(std::map<std::string, std::unique_ptr<Value>> m):
    m_(std::move(m)) {};

public:
  const std::unique_ptr<Value>& at(const std::string &key) const override {
    return m_.at(key);
  }

  std::unique_ptr<Value>& at(const std::string &key) override {
    return m_.at(key);
  }

  const std::unique_ptr<Value>& operator [] (const std::string &key) const override {
    return at(key);
  }

  std::map<std::string, std::unique_ptr<Value>>& as_object() override {
    return m_;
  }

private:
  std::map<std::string, std::unique_ptr<Value>> m_;
};

class ArrayValue: public Value {
public:
  ArrayValue(std::vector<std::unique_ptr<Value>> v) : v_(std::move(v)) {};
  ArrayValue() : v_{} {};

public:
  const std::unique_ptr<Value>& operator [] (unsigned int i) const override {
    return v_[i];
  }

  std::unique_ptr<Value>& operator [] (unsigned int i) override {
    return v_[i];
  }

  void append(std::unique_ptr<Value> v) override {
    v_.push_back(std::move(v));
  }

  std::vector<std::unique_ptr<Value>>& as_array() override {
    return v_;
  }

private:
  std::vector<std::unique_ptr<Value>> v_;
};

class ValueFactory {
public:
  static std::unique_ptr<NumValue>
  create(int n) {
    return std::move(std::unique_ptr<NumValue>(new NumValue(n)));
  }

  static std::unique_ptr<NumValue>
  create(double d) {
    return std::move(std::unique_ptr<NumValue>(new NumValue(d)));
  }

  static std::unique_ptr<StringValue>
  create(const std::string &s) {
    return std::unique_ptr<StringValue>(new StringValue(s));
  }

  static std::unique_ptr<ObjectValue>
  create(std::map<std::string, std::unique_ptr<Value>> m) {
    return std::unique_ptr<ObjectValue>(new ObjectValue(std::move(m)));
  }

  static std::unique_ptr<ArrayValue>
  create(std::vector<std::unique_ptr<Value>> v) {
    return std::unique_ptr<ArrayValue>(new ArrayValue(std::move(v)));
  }
};

#endif /* VALUE_H_ */
