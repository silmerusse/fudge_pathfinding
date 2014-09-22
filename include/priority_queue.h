#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_

template <typename N, typename P>
class PriorityQueue {
public:
  virtual ~PriorityQueue(){};

public:
  // Insert a node to the list according to its priority.
  virtual void insert(const N) = 0;

  // Remove the node with highest priority and return it.
  virtual N remove_front() = 0;

  // Find the node and return its index.
  virtual int find(const N) = 0;

  // Increase the priority of the node in the list.
  virtual void increase_priority(N, P) = 0;

  // Test if the list is empty.
  virtual bool is_empty() = 0;

  // Clear the list.
  virtual void clear() = 0;
};


#endif /* PRIORITY_QUEUE_H_ */
