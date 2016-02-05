#ifndef FUDGE_UP_HEAP_H_
#define FUDGE_UP_HEAP_H_

namespace fudge {

// Percolate up an element at the index specified.
template<typename Container, typename ElementType, typename LessPriority>
static void up_heap(Container &container, int index, LessPriority less_priority) {
  auto value = container[index];
  int hole = index;
  int parent = (hole - 1) / 2;
  while (hole > 0 && less_priority(container[parent], value)) {
    container[hole] = container[parent];
    hole = parent;
    parent = (hole - 1) / 2;
  }
  container[hole] = value;
}

// Percolate up an element at the index specified.
// This is the iterator version.
// Sample:
//   auto it = queue_.begin() + i;
//   up_heap<typename vector<ElementType>::iterator, ElementType,
//     decltype(SP::less_priority)>(queue_.begin(), it, SP::less_priority);
template<typename Iterator, typename ElementType, typename LessPriority>
static void up_heap(Iterator begin, Iterator it, LessPriority less_priority) {
  auto value = *it;
  int hole = it - begin;
  int parent = (hole - 1) / 2;
  while (hole > 0 && less_priority( *(begin + parent), value)) {
    *(begin + hole) = *(begin + parent);
    hole = parent;
    parent = (hole - 1) / 2;
  }
  *(begin + hole) = value;
}

}

#endif /* FUDGE_UP_HEAP_H_ */
