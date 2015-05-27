#ifndef BINARY_HEAP_H_
#define BINARY_HEAP_H_

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "priority_queue.h"

// This is an implementation of binary heap, without using STL's priority queue
// adaptor or heap functions like make_heap(), push_heap(), pop_heap().
template <typename ElementType, typename PriorityType, typename PriorityHandler>
class BinaryHeap : public PriorityQueue<ElementType, PriorityType> {
public:
  BinaryHeap() {}
  virtual ~BinaryHeap() {};

public:
  std::vector<ElementType> queue_;

public:

  void insert(const ElementType e) {
    queue_.push_back(e);
    int i = queue_.size() - 1;
    up(i);
  }

  ElementType remove_front() {
    ElementType front = queue_[0];
    queue_[0] =  queue_.back();
    queue_.pop_back();

    down(0);
    return front;
  }

  ElementType front() {
    ElementType front = queue_[0];
    queue_[0] =  queue_.back();
    return front;
  }

  int find(const ElementType e) {
    auto r = std::find(queue_.begin(), queue_.end(), e);
    if (r == queue_.end())
      return -1;
    else
      return (r - queue_.begin());
  }

  void increase_priority(ElementType e, PriorityType p) {
    int i = find(e);
    PriorityHandler::set_priority(queue_[i], p);
    up(i);
  }

  bool is_empty() {
    return (queue_.size() == 0);
  }

  void clear() {
    queue_.clear();
  }

  // Percolate up an element at the index specified.
  void up(int i) {
    ElementType value = queue_[i];
    int top = 0;
    int hole = i;
    int parent = (hole - 1) / 2;
    while (hole > top &&
        PriorityHandler::less_priority(queue_[parent], value)) {
      queue_[hole] = queue_[parent];
      hole = parent;
      parent = (hole - 1) / 2;
    }
    queue_[hole] = value;
  }

  // Percolate down an element at the index specified.
  void down(int i) {
    int hole = i;
    int len = queue_.size();
    ElementType value = queue_[i];
    int secondChild = 2 * (hole + 1);
    while (secondChild < len) {
      if (PriorityHandler::less_priority(queue_[secondChild],
                                         queue_[secondChild - 1]))
        secondChild--;
      queue_[hole] = queue_[secondChild];
      hole = secondChild;
      secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
      queue_[hole] = queue_[secondChild - 1];
      hole = secondChild - 1;
    }
    queue_[hole] = value;
    up(hole);
  }

  std::size_t size() const override {
    return queue_.size();
  }

};

#endif /* BINARY_HEAP_H_ */
