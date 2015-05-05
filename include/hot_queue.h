#ifndef HOT_QUEUE_H_
#define HOT_QUEUE_H_

#include <cassert>
#include <vector>
#include <algorithm>
#include "priority_queue.h"

using namespace std;

// This data structure consists of two parts. One is a heap called "hot".
// The other is a list of buckets called "cold".
// High priority elements will be put to hot and others will be put to cold.
// When there's no element in hot, threshold increases, and part of the elements
// in cold would be moved to hot.
template <typename ElementType, typename PriorityType, typename PriorityHandler,
                                                             typename HeapType>
class HotQueue
    : public PriorityQueue<ElementType, PriorityType>{
public:
  HotQueue() {};
  HotQueue(PriorityType kc) : kc_(kc) {};

public:
  void insert(const ElementType n) override {
    count_++;
    int i = PriorityHandler::get_priority(n) / kc_;
    if (threshold_ < 0)
      threshold_ = i;

    if (i <= threshold_)
      hot_.insert(n);
    else {
      while (static_cast<int>(cold_.size()) < i - threshold_) {
        vector<ElementType> v;
        cold_.push_back(v);
      }
      cold_[i - threshold_ - 1].push_back(n);
    }
    //count_++;
  }

  ElementType remove_front() override {
    if (hot_.is_empty()) {
      assert (!cold_.empty());

      while (cold_[0].size() == 0) {
        cold_.erase(cold_.begin());
        threshold_++;
      }

      // get the first bucket from cold and heapify it as hot
      std::move(cold_[0].begin(), cold_[0].end(),
          std::back_inserter(hot_.queue_));
      make_heap(hot_.queue_.begin(), hot_.queue_.end(),
                PriorityHandler::less_priority);
      cold_.erase(cold_.begin());

      threshold_++;
    }
    count_--;
    return hot_.remove_front();
  }

  int find(const ElementType n) override {
    int i = PriorityHandler::get_priority(n) / kc_;
    if (i <= threshold_) {
      return hot_.find(n);
    } else {
      int bucket = i - threshold_ - 1;
      auto r = std::find(cold_[bucket].begin(), cold_[bucket].end(), n);
      if (r == cold_[bucket].end())
        return -1;
      else
        return r - cold_[bucket].begin();
    }
  }

  void increase_priority(ElementType n, PriorityType p) override {
    int i = PriorityHandler::get_priority(n) / kc_;
    if (i <= threshold_) {
      hot_.increase_priority(n, p);
    } else {
      erase_from_cold(n);
      PriorityHandler::set_priority(n, p);
      insert(n);
    }
  }

  bool is_empty() override {
    return count_ == 0;
  }

  void clear() override {
    hot_.clear();
    for (auto c : cold_)
      c.clear();
    count_ = 0;
  }

public:
  HeapType hot_;
  vector<vector<ElementType>> cold_; // each level one element is a bucket
  int threshold_ {-1}; // each hot element's priority < (threshold_ + 1)*kc_
  double kc_ = 1.4143; // size of each bucket

private:
  void erase_from_cold(ElementType n) {
    int i = PriorityHandler::get_priority(n) / kc_;
    if (i > threshold_) {
      cold_[i - threshold_ - 1].erase(
          cold_[i - threshold_ - 1].begin() + find(n));
    } else {
      assert(0);
    }
    count_--;
  }

private:
  int count_ = 0;

};


#endif /* HOT_QUEUE_H_ */
