#ifndef PRIORITY_QUEUE_STL_H_
#define PRIORITY_QUEUE_STL_H_

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "priority_queue.h"
#include "up_heap.h"

using namespace std;

// This is an implementation of priority queue based on STL's heap functions
// like make_heap(), push_heap() and pop_heap(). It implemented an
// "increase priority" operation (percolate up), with the assumption that the
// STL implementation uses binary heap(tree) algorithm. An alternate safe but
// slow implementation using make_heap() is also provided by comments.
template <typename ElementType, typename PriorityType, typename PriorityHandler>
class PriorityQueueSTL : public PriorityQueue<ElementType, PriorityType> {
public:
  void insert(ElementType e) override {
    queue_.push_back(e);
    push_heap(queue_.begin(), queue_.end(), PriorityHandler::less_priority);
  }

  ElementType remove_front() override {
    ElementType e = queue_.front();
    pop_heap(queue_.begin(), queue_.end(), PriorityHandler::less_priority);
    queue_.pop_back();
    return e;
  }

  int find(const ElementType e) {
    typename vector<ElementType>::iterator i =
        std::find(queue_.begin(), queue_.end(), e);
    return i - queue_.begin();
  }

  // Here we assume the STL uses a binary heap, and its algorithm matches our
  // implementation of up_heap(). Otherwise, we should use make_heap(), but its
  // complexity is higher and does not respect the previous order (given two
  // elements with same priority):
  //   make_heap(queue_.begin(), queue_.begin() + i + 1, SP::less_priority);
  void increase_priority(ElementType e, PriorityType p) override {
    int i = find(e);
    PriorityHandler::set_priority(queue_[i], p);

    up_heap<vector<ElementType>, ElementType,
                  decltype(PriorityHandler::less_priority)>(queue_, i,
                                            PriorityHandler::less_priority);
  }

  bool is_empty() override {
    return queue_.empty();
  }

  void clear() override {
    queue_.clear();
  }

public:
  vector<ElementType> queue_;
};

#endif /* PRIORITY_QUEUE_STL_H_ */
