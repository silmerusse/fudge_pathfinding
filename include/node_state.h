#ifndef FUDGE_NODE_STATE_H_
#define FUDGE_NODE_STATE_H_

namespace fudge {

enum NodeState {
  unexplored  = 0,
  open        = 1,
  closed      = 2,
  result      = 3,
  start       = 4,
  goal        = 5,
  reserved_1  = 6,
  reserved_2  = 7,
};

}

#endif /* FUDGE_NODE_STATE_H_ */
