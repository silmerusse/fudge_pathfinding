#ifndef FRUITCANDY_JSON_DOCUMENT_H_
#define FRUITCANDY_JSON_DOCUMENT_H_

#include <exception>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <memory>
#include "../util/log.h"
#include "../core/value.h"

class InvalidToken : public std::exception {
public:
  const char* what() const noexcept {
    return "Invalid token.";
  }
};

class EndOfString : public std::exception {
public:
  const char* what() const noexcept {
    return "End of string.";
  }
};

class Document {
public:
  Document() = default;
  virtual ~Document() = default;

public:
  std::unique_ptr<Value> root_;

public:
  bool parse(const std::string &str) {
    try {
      str_ = str;
      skip_whitespaces();
      switch (peek()) {
      case '[':
        root_ = std::move(parse_list());
        return true;
      case '{':
        root_ = std::move(parse_dict());
        return true;
      default:
        throw 1;
      }
    } catch (const std::exception &e) {
      print_error(e);
      return false;
    }
  }

  bool parse(std::ifstream &ifs) {
    std::string str, tmp;
    while(ifs.good()) {
      std::getline(ifs, tmp);
      str += tmp;
    }
    ifs.close();
    return parse(str);
  }

  std::unique_ptr<Value> parse_value() {
    if (peek() == '[')
      return parse_list();
    else if (peek() == '{')
      return parse_dict();
    else if (peek() == '"')
      return parse_string();
    else if (peek() == '.' || peek() == '-' || (peek() >= '0' && peek() <= '9'))
      return parse_number();
    else
      throw 1;
  }

  std::unique_ptr<Value> parse_list() {
    check_leading('[');
    forward(); // skip '['

    std::vector<std::unique_ptr<Value>> vec;
    std::unique_ptr<Value> value(new Value(std::move(vec)));

    while (1) {
      skip_whitespaces();
      if (peek() == ']') {
        forward(); // skip ']'
        return value;
      }
      if (value->length() > 0) {
        skip_comma(); // expect a comma if it's not the first element.
        skip_whitespaces();
      }
      value->append(std::move(parse_value()));
    }
  }

  std::unique_ptr<Value> parse_dict() {
    check_leading('{');
    forward(); // skip '{'

    std::map<std::string, std::unique_ptr<Value>> dict;
    std::unique_ptr<Value> value(new Value(std::move(dict)));

    while (1) {
      skip_whitespaces();
      check_boundary();
      if (peek() == '}') {
        forward(); // skip '}'
        return value;
      }
      if (!value->empty()) { // expect a comma if it's not the first element.
        skip_comma();
        skip_whitespaces();
      }
      value->insert(std::move(parse_pair()));
    }
  }

  std::pair<std::string, std::unique_ptr<Value>> parse_pair() {
    std::string key = parse_string()->as_string();
    skip_whitespaces();
    parse_token(':');
    skip_whitespaces();
    return std::make_pair(key, std::move(parse_value()));
  }

  void parse_token(char c) {
    check_leading(c);
    forward();
  }

  std::unique_ptr<Value> parse_string() {
    check_leading('"');
    forward(); // skip '"'

    std::string s;

    while (!eof() && peek() != '"') {
      s.push_back(peek());
      forward();
    }
    if (!eof())
      forward(); // skip '"'
    else
      throw EndOfString();
    return std::unique_ptr<Value>(new Value(s));
  }

  std::unique_ptr<Value> parse_number() {
    check_boundary();
    if (peek() != '.' && peek() != '-' && (peek() < '0' || peek() > '9'))
      throw InvalidToken();

    std::string s;
    int n;
    double d;

    bool dot_flag = false;
    bool negative_flag = false;

    if (peek() != '-')
      negative_flag = true;

    while (!eof()) {
      if (peek() == '.') {
        if (dot_flag == false) {
          dot_flag = true;
          s.push_back(peek());
          forward();
        } else {
          break;
        }
      } else if (peek() == '-') {
        if (negative_flag == false) {
          negative_flag = true;
          s.push_back(peek());
          forward();
        } else {
          break;
        }
      } else if (peek() >= '0' && peek() <= '9') {
        s.push_back(peek());
        forward();
      } else {
        break;
      }
    }
    if (eof()) {
      throw EndOfString();
    }

    if (dot_flag) {
      std::stringstream (s) >> d;
      return std::unique_ptr<Value>(new Value(d));
    } else {
      std::stringstream (s) >> n;
      return std::unique_ptr<Value>(new Value(n));
    }
  }

  void skip_whitespaces () {
    while (!eof() && (peek() == ' ' || peek() == '\n'
        || peek() == '\r' || peek() == '\t')) {
      forward();
    }
  }

  void skip_comma () {
    if (!eof() && peek() == ',') {
      forward();
    } else {
      throw InvalidToken();
    }
  }

  char peek() {
    return str_[index_];
  }

  void forward() {
    ++index_;
  }

  bool eof() {
    return index_ == static_cast<int>(str_.length());
  }

  void check_boundary() {
    if (eof())
      throw EndOfString();
  }

  void check_leading(char c) {
    check_boundary();
    if (peek() != c) {
      throw InvalidToken();
    }
  }

  void print_error(const std::exception &e) {
    ERROR("parse error: %s", e.what());
    ERROR("%s ^ %s", str_.substr(std::max(index_ - 5, 0), index_).c_str(),
          str_.substr(index_ + 1, std::min(
              static_cast<int>(str_.length() - index_ - 1), 5)).c_str());
  }

  void set(const std::string &str) {
    str_ = str;
  }

private:
  int index_ = 0;
  std::string str_;
};

#endif /* FRUITCANDY_JSON_DOCUMENT_H_ */
