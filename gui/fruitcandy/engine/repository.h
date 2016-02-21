#ifndef FRUITCANDY_ENGINE_REPOSITORY_H_
#define FRUITCANDY_ENGINE_REPOSITORY_H_

#include <functional>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>

#include "item.h"

class Realm;

struct ID{};
struct TAG{};

typedef boost::multi_index_container<
	std::unique_ptr<Item>,
	boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique<
			boost::multi_index::tag<ID>,
			BOOST_MULTI_INDEX_MEMBER(Item, int, id_)
		>,
		boost::multi_index::hashed_non_unique<
			boost::multi_index::tag<TAG>,
			BOOST_MULTI_INDEX_MEMBER(Item, std::string, tag_)
		>
  >
> Items;

// TODO Use better way to generate a new ID.
class Repository {
public:
	Repository(Realm *realm = nullptr) : realm_(realm) {};
	virtual ~Repository() = default;

public:
  template <typename T>
  T* create_item(const std::string &tag) {
    int id = ++max_id_;
    return create_item<T>(id, tag);
  }

  template <typename T>
  T* create_item(int id, const std::string &tag) {
  	std::unique_ptr<T> item(new T());
  	return add_item(id, tag, std::move(item));
  }

  template <typename T>
  T* add_item(const std::string &tag, std::unique_ptr<T> item) {
    int id = ++max_id_;
    return add_item(id, tag, std::move(item));
  }

  template <typename T>
  T* add_item(int id, const std::string &tag, std::unique_ptr<T> item) {
    if (id > max_id_)
      max_id_ = id;
    item->id_ = id;
    item->tag_ = tag;
    T *ptr = item.get();
    items_.insert(std::move(item));
    return ptr;
  }

	template <typename T>
  void traverse(const std::string &tag, std::function<void(T*)> f) {
		Items::index<TAG>::type &index = items_.get<TAG>();
		auto p = index.equal_range(tag);
		auto i = p.first;

		while (i != p.second) {
			f(static_cast<T*>((*i++).get()));
		}
	}

  void traverse_all(std::function<void(Item*)> f) {
		Items::index<ID>::type &index = items_.get<ID>();
		Items::index<ID>::type::iterator i = index.begin();

		while (i != index.end()) {
			f((*i++).get());
		}
	}

  template <typename T>
  T* get_item(int id) {
		typedef Items::index<ID>::type BY_ID;
		BY_ID &index = items_.get<ID>();
		BY_ID::iterator i = index.find(id);
		return static_cast<T*>((*i).get());
  }
protected:
	Items items_;
	int max_id_ = -1;
  Realm *realm_ = nullptr;
};

#endif /* FRUITCANDY_ENGINE_REPOSITORY_H_ */
