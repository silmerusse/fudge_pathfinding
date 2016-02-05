#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_

namespace fudge {

template <typename N, typename P>
class PriorityQueue {
public:
  virtual ~PriorityQueue(){};

public:
  // Insert a node to the list according to its priority.
  virtual void insert(const N &n) = 0;

  // Remove the node with highest priority and return it.
  virtual N remove_front() = 0;

  // Return node with highest priority.
  virtual N front() = 0;

  // Find the node and return its index.
  virtual int find(const N &n) = 0;

  // Increase the priority of the node in the list.
  virtual void increase_priority(const N &n, P p) = 0;

  // Test if the list is empty.
  virtual bool is_empty() const = 0;

  // Clear the list.
  virtual void clear() = 0;

  // Count of nodes.
  virtual std::size_t size() const = 0;
};

}

#endif /* PRIORITY_QUEUE_H_ */
