#ifndef NODE_STATE_H_
#define NODE_STATE_H_

enum NodeState {
  unexplored  = 0,
  open        = 1,
  closed      = 2,
  result      = 3,
  start       = 4,
  goal        = 5,
};

#endif /* NODE_STATE_H_ */
