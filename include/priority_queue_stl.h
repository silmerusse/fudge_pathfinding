#ifndef FUDGE_PRIORITY_QUEUE_STL_H_
#define FUDGE_PRIORITY_QUEUE_STL_H_

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "priority_queue.h"
#include "up_heap.h"

// This is an implementation of priority queue based on STL's heap functions
// like make_heap(), push_heap() and pop_heap(). It implements an
// "increase priority" operation (percolate up), with the assumption that the
// STL implementation uses binary heap(tree) algorithm. An alternate safe but
// slow implementation using make_heap() is also provided for reference.

namespace fudge {

template <typename ElementType, typename PriorityType, typename PriorityHandler>
class PriorityQueueSTL : public PriorityQueue<ElementType, PriorityType> {
public:
  PriorityQueueSTL() = default;
  virtual ~PriorityQueueSTL() = default;

public:
  virtual void insert(const ElementType &e) override {
    queue_.push_back(e);
    push_heap(queue_.begin(), queue_.end(), PriorityHandler::less_priority);
  }

  virtual ElementType remove_front() override {
    ElementType e = queue_.front();
    pop_heap(queue_.begin(), queue_.end(), PriorityHandler::less_priority);
    queue_.pop_back();
    return e;
  }

  virtual ElementType front() override {
    ElementType e = queue_.front();
    return e;
  }

  virtual int find(const ElementType &e) override {
    typename std::vector<ElementType>::iterator i =
        std::find(queue_.begin(), queue_.end(), e);
    if (i == queue_.end())
      return -1;
    else
      return i - queue_.begin();
  }

  // Here we assume the STL uses a binary heap, and its algorithm matches our
  // implementation of up_heap(). Otherwise, we should use make_heap(), but its
  // complexity is higher and does not respect the previous order (given two
  // elements with same priority):
  //   make_heap(queue_.begin(), queue_.begin() + i + 1, SP::less_priority);
  virtual void increase_priority(const ElementType &e,
                                 PriorityType p) override {
    int i = find(e);
    PriorityHandler::set_priority(queue_[i], p);

    up_heap<std::vector<ElementType>, ElementType,
        decltype(PriorityHandler::less_priority)>(
            queue_, i, PriorityHandler::less_priority);
  }

  virtual bool is_empty() const override {
    return queue_.empty();
  }

  virtual void clear() override {
    queue_.clear();
  }

  virtual std::size_t size() const override {
    return queue_.size();
  }

public:
  std::vector<ElementType> queue_;
};

}

#endif /* FUDGE_PRIORITY_QUEUE_STL_H_ */
