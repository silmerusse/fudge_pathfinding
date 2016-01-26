#include <functional>
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "multi_agent_map.h"
#include "astar_search.h"
#include "util/time_util.h"
#include "load_matrix.h"

using NodeType = std::shared_ptr<MultiAgentNode>;

// Strip the path. Only keep the end of turn nodes.
std::vector<const MultiAgentNode*> strip_path(
    const std::vector<NodeType> &path,
    MultiAgentNode *start) {
  std::vector<const MultiAgentNode*> result;
  start->annotation_ = start->to_string();
  result.push_back(start);

  for (auto i = path.rbegin(); i != path.rend(); ++i)
    if ((*i)->end_of_turn_)
      result.push_back((*i).get());
  return result;
}

// Helper to print and return result.
void print_result(const std::vector<const MultiAgentNode*> &path,
                  const MultiAgentMap &map){
  std::cout << "--------------------\n";
  std::ostringstream ss;
  for (auto i = path.begin(); i != path.end(); ++i)
    ss << (*i)->annotation_  << " --- " << (*i)->cost_ << std::endl;
  ss << map.stats_.to_string() << std::endl;;
  std::cout << ss.str();
}

// Test if hash code works.
TEST(MultiAgentMap, hash) {
  auto n0 = MultiAgentNode::create({}, {},
                          {Agent(0, Pos(0, 0), Pos(1, 0), 1),
                           Agent(1, Pos(0, 0), Pos(1, 0), 1)});
  auto n1 = MultiAgentNode::create({},
           {Move(Agent(0, Pos(0, 0), Pos(1, 0), 1), Pos(1, 0)),
            Move(Agent(0, Pos(0, 0), Pos(1, 0), 1), Pos(0, 1))},
           {Agent(0, Pos(0, 0), Pos(1, 0), 1),
            Agent (1, Pos(0, 0), Pos(1, 0), 1)});
  ASSERT_EQ(n0->hash(), n1->hash());

  auto n2 = MultiAgentNode::create({},
           {Move(Agent(0, Pos(0, 0), Pos(1, 0), 1), Pos(1, 0))},
           {Agent(0, Pos(0, 0), Pos(1, 0), 1),
            Agent (1, Pos(0, 0), Pos(1, 0), 1)});
  ASSERT_EQ(n0->hash(), n2->hash());

  auto n3 = MultiAgentNode::create({
    Move(Agent(0, Pos(0, 0), Pos(1, 0), 1), Pos(1, 0))},
    {},
    {Agent(0, Pos(0, 0), Pos(1, 0), 1),
     Agent (1, Pos(0, 0), Pos(1, 0), 1)});
  ASSERT_NE(n0->hash(), n3->hash());

  auto n4 = MultiAgentNode::create({},{},
             {Agent(3, Pos(2, 3), Pos(2, 2), 1),
              Agent(4, Pos(4, 1), Pos(2, 2), 1),
              Agent(5, Pos(5, 8), Pos(2, 2), 1),
             });
  auto n5 = MultiAgentNode::create({}, {},
             {Agent(3, Pos(2, 3), Pos(2, 2), 1),
              Agent(4, Pos(4, 1), Pos(2, 2), 1),
              Agent(5, Pos(5, 8), Pos(2, 2), 1),});
  ASSERT_EQ(n4->hash(), n5->hash());
}

// Two agents start at the same top-left corner.
// Test if the edges could be generated correctly.
// Also test the annotation is represented correctly.
TEST(MultiAgentMap, edges) {
  std::vector<int> matrix {
    1, 1,
    1, 1
  };

  MultiAgentMap map(2, 2, matrix);

  auto start = MultiAgentNode::create({}, {},
                          {Agent(0, Pos(0, 0), Pos(1, 0), 1),
                           Agent (1, Pos(0, 0), Pos(1, 0), 1)});
  auto end = MultiAgentNode::create();
  map.open_node(start, 0, map.heuristic_rra(start, end), start);

  auto node = start;
  auto e = map.edges(node).front();

  ASSERT_EQ("[A0|(0,0)->(1,0)]<>{B0|(0,0)}", e.to_->to_string());
}

// Only 2 grids.
// Test if two nodes could run into target one by one.
TEST(MultiAgentMap, search_2x1_2_agents) {
  std::vector<int> matrix {1, 1};

  MultiAgentMap map(2, 1, matrix);

  auto start = MultiAgentNode::create({}, {},
                          {Agent(0, Pos(0, 0), Pos(1, 0), 1),
                           Agent (1, Pos(0, 0), Pos(1, 0), 1)});
  auto end = MultiAgentNode::create();
  auto start0 = *start;

  PREPARE_TIMER
  START_TIMER
  std::vector<NodeType> &&path0 = AStarSearch::search(map,
          start, end,
          std::bind(&MultiAgentMap::heuristic_rra, &map,
                    std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  auto &&path = strip_path(path0, &start0);
  print_result(path, map);

  /*
  []<>{A0|(0,0) B0|(0,0)} --- 0
  [A0|(0,0)->(0,0) B0|(0,0)->(1,0)]<>{} --- 3
  [A0|(0,0)->(1,0)]<>{} --- 3
  */
  ASSERT_EQ(3, path.size());
}


// One faster agent has to wait another slower node to move.
// Test if predecessor assignment works.
TEST(MultiAgentMap, search_2x1_2_agents_chain) {

  std::vector<int> matrix {1, 1};

  MultiAgentMap map(2, 1, matrix);

  auto start = MultiAgentNode::create({}, {},
                          {Agent(0, Pos(0, 0), Pos(1, 0), 1, 1),
                           Agent (1, Pos(0, 0), Pos(1, 0), 2)});
  auto end = MultiAgentNode::create();
  auto start0 = *start;

  PREPARE_TIMER
  START_TIMER
  std::vector<NodeType> &&path0 = AStarSearch::search(map,
          start, end,
          std::bind(&MultiAgentMap::heuristic_rra, &map,
                    std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  auto &&path = strip_path(path0, &start0);
  print_result(path, map);
/*
  []<>{A0|(0,0) B1|(0,0)} --- 0
  [A0|(0,0)->(0,0) B0|(0,0)->(0,0)]<>{} --- 5
  [A0|(0,0)->(0,0) B1|(0,0)->(1,0)]<>{} --- 5
  [A0|(0,0)->(1,0)]<>{} --- 5
*/
  ASSERT_EQ(4, path.size());
}

// Test two nodes will not enter the same target grid at the same time.
TEST(MultiAgentMap, search_2x2_2_agents) {
  std::vector<int> matrix {1, 1, 1, 1};

  MultiAgentMap map(2, 2, matrix);

  auto start = MultiAgentNode::create({}, {},
                          {Agent(0, Pos(0, 0), Pos(1, 1), 1),
                           Agent (1, Pos(0, 0), Pos(1, 1), 1)});
  auto end = MultiAgentNode::create();
  auto start0 = *start;

  PREPARE_TIMER
  START_TIMER
  std::vector<NodeType> &&path0 = AStarSearch::search(map,
          start, end,
          std::bind(&MultiAgentMap::heuristic_rra, &map,
                    std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  auto &&path = strip_path(path0, &start0);
  print_result(path, map);

  /*
  []<>{A0|(0,0) B0|(0,0)} --- 0
  [A0|(0,0)->(0,1) B0|(0,0)->(1,0)]<>{} --- 4
  [A0|(0,1)->(1,1) B0|(1,0)->(1,0)]<>{} --- 5
  [B0|(1,0)->(1,1)]<>{} --- 5
   */
  ASSERT_EQ(4, path.size());
}

// Test 3 agents in 2 groups, which have different start and end grids.
TEST(MultiAgentMap, search_2x2_3_agents) {
  std::vector<int> matrix {1, 1, 1, 1};

  MultiAgentMap map(2, 2, matrix);

  auto start = MultiAgentNode::create({}, {},
                          {Agent(0, Pos(0, 0), Pos(1, 1), 1),
                           Agent (1, Pos(0, 0), Pos(1, 1), 1),
                           Agent (2, Pos(1, 1), Pos(0, 0), 1)});
  auto end = MultiAgentNode::create();
  auto start0 = *start;

  PREPARE_TIMER
  START_TIMER
  std::vector<NodeType> &&path0 = AStarSearch::search(map,
          start, end,
          std::bind(&MultiAgentMap::heuristic_rra, &map,
                    std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  auto &&path = strip_path(path0, &start0);
  print_result(path, map);

  /*
  []<>{A0|(0,0) B0|(0,0) C0|(1,1)} --- 0
  [A0|(0,0)->(0,1) B0|(0,0)->(0,0) C0|(1,1)->(1,0)]<>{} --- 7
  [A0|(0,1)->(1,1) B0|(0,0)->(0,1) C0|(1,0)->(0,0)]<>{} --- 7
  [B0|(0,1)->(1,1)]<>{} --- 7
   */
  ASSERT_EQ(4, path.size());
}

// Test 1 agent finding path in a plain 10x10 map.
// The result should be the same as a normal a* search.
TEST(MultiAgentMap, search_10x10_1_agent) {

  std::vector<int> matrix = load_matrix<int>("../data/matrix_10x10_plain.txt");

  MultiAgentMap map(10, 10, matrix);

  auto start = MultiAgentNode::create({}, {},
                          {Agent(0, Pos(0, 0), Pos(9, 9), 1),
                          /* Agent (1, Pos(0, 0), Pos(9, 9), 1)*/});
  auto end = MultiAgentNode::create();
  auto start0 = *start;

  PREPARE_TIMER
  START_TIMER
  std::vector<NodeType> &&path0 = AStarSearch::search(map,
          start, end,
          std::bind(&MultiAgentMap::heuristic_rra, &map,
                    std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  auto &&path = strip_path(path0, &start0);
  print_result(path, map);
  /*
  []<>{A0|(0,0)} --- 0
  [A0|(0,0)->(1,0)]<>{} --- 18
  [A0|(1,0)->(2,0)]<>{} --- 18
  [A0|(2,0)->(3,0)]<>{} --- 18
  [A0|(3,0)->(3,1)]<>{} --- 18
  [A0|(3,1)->(3,2)]<>{} --- 18
  [A0|(3,2)->(3,3)]<>{} --- 18
  [A0|(3,3)->(3,4)]<>{} --- 18
  [A0|(3,4)->(3,5)]<>{} --- 18
  [A0|(3,5)->(3,6)]<>{} --- 18
  [A0|(3,6)->(3,7)]<>{} --- 18
  [A0|(3,7)->(3,8)]<>{} --- 18
  [A0|(3,8)->(3,9)]<>{} --- 18
  [A0|(3,9)->(4,9)]<>{} --- 18
  [A0|(4,9)->(5,9)]<>{} --- 18
  [A0|(5,9)->(6,9)]<>{} --- 18
  [A0|(6,9)->(7,9)]<>{} --- 18
  [A0|(7,9)->(8,9)]<>{} --- 18
  [A0|(8,9)->(9,9)]<>{} --- 18
   */
  ASSERT_EQ(19, path.size());
}

// Test 1 agent finding path in a 10x10 map with obstacles.
// The result should be the same as a normal a* search.
TEST(MultiAgentMap, search_10x10_wall_1_agent) {

  std::vector<int> matrix = load_matrix<int>("../data/matrix_10x10_agents.txt");

  MultiAgentMap map(10, 10, matrix);

  auto start = MultiAgentNode::create({}, {},
                          {Agent(0, Pos(0, 0), Pos(9, 9), 1),
                          /* Agent (1, Pos(0, 0), Pos(9, 9), 1)*/});
  auto end = MultiAgentNode::create();
  auto start0 = *start;

  PREPARE_TIMER
  START_TIMER
  std::vector<NodeType> &&path0 = AStarSearch::search(map,
          start, end,
          std::bind(&MultiAgentMap::heuristic_rra, &map,
                    std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  auto &&path = strip_path(path0, &start0);
  print_result(path, map);
  /*
  []<>{A0|(0,0)} --- 0
  [A0|(0,0)->(1,0)]<>{} --- 18
  [A0|(1,0)->(2,0)]<>{} --- 18
  [A0|(2,0)->(2,1)]<>{} --- 18
  [A0|(2,1)->(2,2)]<>{} --- 18
  [A0|(2,2)->(2,3)]<>{} --- 18
  [A0|(2,3)->(2,4)]<>{} --- 18
  [A0|(2,4)->(2,5)]<>{} --- 18
  [A0|(2,5)->(2,6)]<>{} --- 18
  [A0|(2,6)->(2,7)]<>{} --- 18
  [A0|(2,7)->(2,8)]<>{} --- 18
  [A0|(2,8)->(3,8)]<>{} --- 18
  [A0|(3,8)->(3,9)]<>{} --- 18
  [A0|(3,9)->(4,9)]<>{} --- 18
  [A0|(4,9)->(5,9)]<>{} --- 18
  [A0|(5,9)->(6,9)]<>{} --- 18
  [A0|(6,9)->(7,9)]<>{} --- 18
  [A0|(7,9)->(8,9)]<>{} --- 18
  [A0|(8,9)->(9,9)]<>{} --- 18
   */
  ASSERT_EQ(19, path.size());
}

// Test 2 agents finding paths in a 10x10 map with obstacles.
TEST(MultiAgentMap, search_10x10_wall_2_agents) {
  std::vector<int> matrix = load_matrix<int>("../data/matrix_10x10_agents.txt");

  MultiAgentMap map(10, 10, matrix);

  auto start = MultiAgentNode::create({}, {},
                          {Agent(0, Pos(0, 0), Pos(9, 9), 1),
                           Agent (1, Pos(0, 0), Pos(9, 9), 1)});
  auto end = MultiAgentNode::create();
  auto start0 = *start;

  PREPARE_TIMER
  START_TIMER
  std::vector<NodeType> &&path0 = AStarSearch::search(map,
          start, end,
          std::bind(&MultiAgentMap::heuristic_rra, &map,
                    std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  auto &&path = strip_path(path0, &start0);
  print_result(path, map);
  /*
  []<>{A0|(0,0) B0|(0,0)} --- 0
  [A0|(0,0)->(1,0) B0|(0,0)->(0,1)]<>{} --- 36
  [A0|(1,0)->(1,1) B0|(0,1)->(0,2)]<>{} --- 36
  [A0|(1,1)->(2,1) B0|(0,2)->(1,2)]<>{} --- 36
  [A0|(2,1)->(2,2) B0|(1,2)->(1,3)]<>{} --- 36
  [A0|(2,2)->(2,3) B0|(1,3)->(1,4)]<>{} --- 36
  [A0|(2,3)->(2,4) B0|(1,4)->(1,5)]<>{} --- 36
  [A0|(2,4)->(2,5) B0|(1,5)->(1,6)]<>{} --- 36
  [A0|(2,5)->(2,6) B0|(1,6)->(1,7)]<>{} --- 36
  [A0|(2,6)->(2,6) B0|(1,7)->(2,7)]<>{} --- 37
  [A0|(2,6)->(2,7) B0|(2,7)->(3,7)]<>{} --- 37
  [A0|(2,7)->(3,7) B0|(3,7)->(3,8)]<>{} --- 37
  [A0|(3,7)->(4,7) B0|(3,8)->(4,8)]<>{} --- 37
  [A0|(4,7)->(4,8) B0|(4,8)->(5,8)]<>{} --- 37
  [A0|(4,8)->(5,8) B0|(5,8)->(6,8)]<>{} --- 37
  [A0|(5,8)->(6,8) B0|(6,8)->(7,8)]<>{} --- 37
  [A0|(6,8)->(7,8) B0|(7,8)->(8,8)]<>{} --- 37
  [A0|(7,8)->(8,8) B0|(8,8)->(9,8)]<>{} --- 37
  [A0|(8,8)->(9,8) B0|(9,8)->(9,9)]<>{} --- 37
  [A0|(9,8)->(9,9)]<>{} --- 37
   */
  ASSERT_EQ(20, path.size());
}

// Test 2 agents finding paths in different speed.
TEST(MultiAgentMap, search_2x2_2_agents_different_speed) {
  std::vector<int> matrix {1, 1, 1, 1};

  MultiAgentMap map(2, 2, matrix);

  auto start = MultiAgentNode::create({}, {},
                          {Agent(0, Pos(0, 0), Pos(1, 1), 1),
                           Agent (1, Pos(0, 0), Pos(1, 1), 2)});
  auto end = MultiAgentNode::create();
  auto start0 = *start;

  PREPARE_TIMER
  START_TIMER
  std::vector<NodeType> &&path0 = AStarSearch::search(map,
          start, end,
          std::bind(&MultiAgentMap::heuristic_rra, &map,
                    std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  auto &&path = strip_path(path0, &start0);
  print_result(path, map);
  /*
  []<>{A0|(0,0) B1|(0,0)} --- 0
  [A0|(0,0)->(1,0) B0|(0,0)->(0,0)]<>{} --- 7
  [A0|(1,0)->(1,1) B1|(0,0)->(0,1)]<>{} --- 6
  [B0|(0,1)->(0,1)]<>{} --- 7
  [B1|(0,1)->(1,1)]<>{} --- 6
  */
  ASSERT_EQ(5, path.size());
}

// Test 2 agents finding paths in different slower speed.
TEST(MultiAgentMap, search_2x2_2_agents_different_speed_slower) {
  std::vector<int> matrix {1, 1, 1, 1};

  MultiAgentMap map(2, 2, matrix);

  auto start = MultiAgentNode::create({}, {},
                          {Agent(0, Pos(0, 0), Pos(1, 1), 2),
                           Agent (1, Pos(0, 0), Pos(1, 1), 3)});
  auto end = MultiAgentNode::create();
  auto start0 = *start;

  PREPARE_TIMER
  START_TIMER
  std::vector<NodeType> &&path0 = AStarSearch::search(map,
          start, end,
          std::bind(&MultiAgentMap::heuristic_rra, &map,
                    std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  auto &&path = strip_path(path0, &start0);
  print_result(path, map);
  /*
  []<>{A1|(0,0) B2|(0,0)} --- 0
  [A0|(0,0)->(0,0) B1|(0,0)->(0,0)]<>{} --- 12
  [A1|(0,0)->(0,1) B0|(0,0)->(0,0)]<>{} --- 12
  [A0|(0,1)->(0,1) B2|(0,0)->(1,0)]<>{} --- 11
  [A1|(0,1)->(1,1) B1|(1,0)->(1,0)]<>{} --- 11
  [B0|(1,0)->(1,0)]<>{} --- 12
  [B2|(1,0)->(1,1)]<>{} --- 10
  */
  ASSERT_EQ(7, path.size());
}

// Test 2 agents finding paths in different speed in a 10x10 map with obstacles.
TEST(MultiAgentMap, search_10x10_2_agents_different_speed) {
  std::vector<int> matrix = load_matrix<int>("../data/matrix_10x10_agents.txt");

  MultiAgentMap map(10, 10, matrix);

  auto start = MultiAgentNode::create({}, {},
                          {Agent(0, Pos(0, 0), Pos(9, 9), 1),
                           Agent (1, Pos(0, 0), Pos(9, 9), 2)});
  auto end = MultiAgentNode::create();
  auto start0 = *start;

  PREPARE_TIMER
  START_TIMER
  std::vector<NodeType> &&path0 = AStarSearch::search(map,
          start, end,
          std::bind(&MultiAgentMap::heuristic_rra, &map,
                    std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  auto &&path = strip_path(path0, &start0);
  print_result(path, map);
  /*
  []<>{A0|(0,0) B1|(0,0)} --- 0
  [A0|(0,0)->(1,0) B0|(0,0)->(0,0)]<>{} --- 55
  [A0|(1,0)->(2,0) B1|(0,0)->(1,0)]<>{} --- 54
  [A0|(2,0)->(2,1) B0|(1,0)->(1,0)]<>{} --- 55
  [A0|(2,1)->(2,2) B1|(1,0)->(2,0)]<>{} --- 54
  [A0|(2,2)->(2,3) B0|(2,0)->(2,0)]<>{} --- 55
  [A0|(2,3)->(2,4) B1|(2,0)->(2,1)]<>{} --- 54
  [A0|(2,4)->(2,5) B0|(2,1)->(2,1)]<>{} --- 55
  [A0|(2,5)->(2,6) B1|(2,1)->(2,2)]<>{} --- 54
  [A0|(2,6)->(2,7) B0|(2,2)->(2,2)]<>{} --- 55
  [A0|(2,7)->(3,7) B1|(2,2)->(2,3)]<>{} --- 54
  [A0|(3,7)->(3,8) B0|(2,3)->(2,3)]<>{} --- 55
  [A0|(3,8)->(4,8) B1|(2,3)->(2,4)]<>{} --- 54
  [A0|(4,8)->(5,8) B0|(2,4)->(2,4)]<>{} --- 55
  [A0|(5,8)->(5,9) B1|(2,4)->(2,5)]<>{} --- 54
  [A0|(5,9)->(6,9) B0|(2,5)->(2,5)]<>{} --- 55
  [A0|(6,9)->(7,9) B1|(2,5)->(2,6)]<>{} --- 54
  [A0|(7,9)->(8,9) B0|(2,6)->(2,6)]<>{} --- 55
  [A0|(8,9)->(9,9) B1|(2,6)->(2,7)]<>{} --- 54
  [B0|(2,7)->(2,7)]<>{} --- 55
  [B1|(2,7)->(3,7)]<>{} --- 54
  [B0|(3,7)->(3,7)]<>{} --- 55
  [B1|(3,7)->(4,7)]<>{} --- 54
  [B0|(4,7)->(4,7)]<>{} --- 55
  [B1|(4,7)->(4,8)]<>{} --- 54
  [B0|(4,8)->(4,8)]<>{} --- 55
  [B1|(4,8)->(5,8)]<>{} --- 54
  [B0|(5,8)->(5,8)]<>{} --- 55
  [B1|(5,8)->(5,9)]<>{} --- 54
  [B0|(5,9)->(5,9)]<>{} --- 55
  [B1|(5,9)->(6,9)]<>{} --- 54
  [B0|(6,9)->(6,9)]<>{} --- 55
  [B1|(6,9)->(7,9)]<>{} --- 54
  [B0|(7,9)->(7,9)]<>{} --- 55
  [B1|(7,9)->(8,9)]<>{} --- 54
  [B0|(8,9)->(8,9)]<>{} --- 55
  [B1|(8,9)->(9,9)]<>{} --- 54
   */
  ASSERT_EQ(37, path.size());
}

// Test 6 agents in 2 groups finding paths in different speed in a 10x10 map
// with obstacles.
TEST(MultiAgentMap, search_10x10_3_agents) {
  std::vector<int> matrix = load_matrix<int>("../data/matrix_10x10_agents.txt");
  //std::vector<int> matrix = load_matrix<int>("../data/matrix_10x10_plain.txt");

  MultiAgentMap map(10, 10, matrix);

  auto start = MultiAgentNode::create({}, {},
                          {
                           Agent(3, Pos(0, 0), Pos(9, 9), 1),
                           Agent(4, Pos(0, 0), Pos(9, 9), 1),
                           Agent(5, Pos(9, 9), Pos(0, 0), 1),
                          });
  auto end = MultiAgentNode::create();
  auto start0 = *start;

  PREPARE_TIMER
  START_TIMER
  std::vector<NodeType> &&path0 = AStarSearch::search(map,
          start, end,
          std::bind(&MultiAgentMap::heuristic_rra, &map,
                    std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  auto &&path = strip_path(path0, &start0);
  print_result(path, map);

  /*
  []<>{D0|(0,0) E0|(0,0) F0|(9,9)} --- 0
  [D0|(0,0)->(1,0) E0|(0,0)->(0,1) F0|(9,9)->(8,9)]<>{} --- 54
  [D0|(1,0)->(2,0) E0|(0,1)->(0,2) F0|(8,9)->(7,9)]<>{} --- 54
  [D0|(2,0)->(3,0) E0|(0,2)->(0,3) F0|(7,9)->(6,9)]<>{} --- 54
  [D0|(3,0)->(4,0) E0|(0,3)->(0,4) F0|(6,9)->(5,9)]<>{} --- 54
  [D0|(4,0)->(4,1) E0|(0,4)->(1,4) F0|(5,9)->(4,9)]<>{} --- 54
  [D0|(4,1)->(4,2) E0|(1,4)->(2,4) F0|(4,9)->(3,9)]<>{} --- 54
  [D0|(4,2)->(4,3) E0|(2,4)->(2,5) F0|(3,9)->(2,9)]<>{} --- 54
  [D0|(4,3)->(4,4) E0|(2,5)->(3,5) F0|(2,9)->(2,8)]<>{} --- 54
  [D0|(4,4)->(5,4) E0|(3,5)->(4,5) F0|(2,8)->(2,7)]<>{} --- 54
  [D0|(5,4)->(6,4) E0|(4,5)->(4,6) F0|(2,7)->(2,6)]<>{} --- 54
  [D0|(6,4)->(6,5) E0|(4,6)->(4,7) F0|(2,6)->(2,5)]<>{} --- 54
  [D0|(6,5)->(6,6) E0|(4,7)->(4,8) F0|(2,5)->(1,5)]<>{} --- 54
  [D0|(6,6)->(6,7) E0|(4,8)->(5,8) F0|(1,5)->(1,4)]<>{} --- 54
  [D0|(6,7)->(7,7) E0|(5,8)->(6,8) F0|(1,4)->(1,3)]<>{} --- 54
  [D0|(7,7)->(8,7) E0|(6,8)->(7,8) F0|(1,3)->(1,2)]<>{} --- 54
  [D0|(8,7)->(9,7) E0|(7,8)->(8,8) F0|(1,2)->(1,1)]<>{} --- 54
  [D0|(9,7)->(9,8) E0|(8,8)->(8,9) F0|(1,1)->(1,0)]<>{} --- 54
  [D0|(9,8)->(9,9) E0|(8,9)->(8,9) F0|(1,0)->(0,0)]<>{} --- 55
  [E0|(8,9)->(9,9)]<>{} --- 55
  */
  ASSERT_EQ(20, path.size());
}

// Test 6 agents in 2 groups finding paths in different speed in a 10x10 map
// with obstacles.
// All agents except the first one of each group have predecessors.
TEST(MultiAgentMap, search_10x10_6_agents) {
  std::vector<int> matrix = load_matrix<int>("../data/matrix_10x10_agents.txt");
  //std::vector<int> matrix = load_matrix<int>("../data/matrix_10x10_plain.txt");

  // Increase heuristic weight to accelerate searching.
  // But the result may not be optimal.
  MultiAgentMap map(10, 10, matrix, 1.25);

  auto start = MultiAgentNode::create({}, {},
      {Agent(0, Pos(0, 0), Pos(9, 9), 3),
       Agent(1, Pos(0, 0), Pos(9, 9), 2, 0),
       Agent(2, Pos(0, 0), Pos(9, 9), 1, 1),
       Agent(5, Pos(9, 9), Pos(0, 0), 3),
       Agent(6, Pos(9, 9), Pos(0, 0), 2, 5),
       Agent(7, Pos(9, 9), Pos(0, 0), 1, 6),
      });
  auto end = MultiAgentNode::create();
  auto start0 = *start;

  PREPARE_TIMER
  START_TIMER
  std::vector<NodeType> &&path0 = AStarSearch::search(map,
          start, end,
          std::bind(&MultiAgentMap::heuristic_rra, &map,
                    std::placeholders::_1, std::placeholders::_2));
  END_TIMER
  PRINT_TIME_ELAPSED

  auto &&path = strip_path(path0, &start0);
  print_result(path, map);

  ASSERT_LE(55, path.size());
}


