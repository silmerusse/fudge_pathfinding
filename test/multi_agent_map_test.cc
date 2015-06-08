#include <functional>
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "multi_agent_map.h"
#include "astar_search.h"
#include "time_util.h"
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

// Test 3 agents in 2 groups finding paths in different speed in a 10x10 map
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

// Test 10 agents in 2 groups finding paths in different speed in a 10x10 map
// with obstacles.
// All agents except the first one of each group have predecessors.
TEST(MultiAgentMap, search_10x10_10_agents) {
  std::vector<int> matrix = load_matrix<int>("../data/matrix_10x10_agents.txt");
  //std::vector<int> matrix = load_matrix<int>("../data/matrix_10x10_plain.txt");

  // Increase heuristic weight to accelerate searching.
  // But the result may not be optimal.
  MultiAgentMap map(10, 10, matrix, 1.25);

  auto start = MultiAgentNode::create({}, {},
      {Agent(0, Pos(0, 0), Pos(9, 9), 5),
       Agent(1, Pos(0, 0), Pos(9, 9), 4, 0),
       Agent(2, Pos(0, 0), Pos(9, 9), 3, 1),
       Agent(3, Pos(0, 0), Pos(9, 9), 2, 2),
       Agent(4, Pos(0, 0), Pos(9, 9), 1, 3),
       Agent(5, Pos(9, 9), Pos(0, 0), 5),
       Agent(6, Pos(9, 9), Pos(0, 0), 4, 5),
       Agent(7, Pos(9, 9), Pos(0, 0), 3, 6),
       Agent(8, Pos(9, 9), Pos(0, 0), 2, 7),
       Agent(9, Pos(9, 9), Pos(0, 0), 1, 8),
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

  ASSERT_LE(94, path.size());

  /*
  []<>{A4|(0,0) B3|(0,0) C2|(0,0) D1|(0,0) E0|(0,0) F4|(9,9) G3|(9,9) H2|(9,9) I1|(9,9) J0|(9,9)} --- 0
  [A3|(0,0)->(0,0) B2|(0,0)->(0,0) C1|(0,0)->(0,0) D0|(0,0)->(0,0) E0|(0,0)->(0,0) F3|(9,9)->(9,9) G2|(9,9)->(9,9) H1|(9,9)->(9,9) I0|(9,9)->(9,9) J0|(9,9)->(9,9)]<>{} --- 685
  [A2|(0,0)->(0,0) B1|(0,0)->(0,0) C0|(0,0)->(0,0) D0|(0,0)->(0,0) E0|(0,0)->(0,0) F2|(9,9)->(9,9) G1|(9,9)->(9,9) H0|(9,9)->(9,9) I0|(9,9)->(9,9) J0|(9,9)->(9,9)]<>{} --- 695
  [A1|(0,0)->(0,0) B0|(0,0)->(0,0) C0|(0,0)->(0,0) D0|(0,0)->(0,0) E0|(0,0)->(0,0) F1|(9,9)->(9,9) G0|(9,9)->(9,9) H0|(9,9)->(9,9) I0|(9,9)->(9,9) J0|(9,9)->(9,9)]<>{} --- 705
  [A0|(0,0)->(0,0) B0|(0,0)->(0,0) C0|(0,0)->(0,0) D0|(0,0)->(0,0) E0|(0,0)->(0,0) F0|(9,9)->(9,9) G0|(9,9)->(9,9) H0|(9,9)->(9,9) I0|(9,9)->(9,9) J0|(9,9)->(9,9)]<>{} --- 715
  [A4|(0,0)->(1,0) B0|(0,0)->(0,0) C0|(0,0)->(0,0) D0|(0,0)->(0,0) E0|(0,0)->(0,0) F4|(9,9)->(8,9) G0|(9,9)->(9,9) H0|(9,9)->(9,9) I0|(9,9)->(9,9) J0|(9,9)->(9,9)]<>{} --- 712
  [A3|(1,0)->(1,0) B3|(0,0)->(0,1) C0|(0,0)->(0,0) D0|(0,0)->(0,0) E0|(0,0)->(0,0) F3|(8,9)->(8,9) G0|(9,9)->(9,9) H0|(9,9)->(9,9) I0|(9,9)->(9,9) J0|(9,9)->(9,9)]<>{} --- 717
  [A2|(1,0)->(1,0) B2|(0,1)->(0,1) C0|(0,0)->(0,0) D0|(0,0)->(0,0) E0|(0,0)->(0,0) F2|(8,9)->(8,9) G0|(9,9)->(9,9) H0|(9,9)->(9,9) I0|(9,9)->(9,9) J0|(9,9)->(9,9)]<>{} --- 727
  [A1|(1,0)->(1,0) B1|(0,1)->(0,1) C0|(0,0)->(0,0) D0|(0,0)->(0,0) E0|(0,0)->(0,0) F1|(8,9)->(8,9) G3|(9,9)->(9,8) H0|(9,9)->(9,9) I0|(9,9)->(9,9) J0|(9,9)->(9,9)]<>{} --- 732
  [A0|(1,0)->(1,0) B0|(0,1)->(0,1) C0|(0,0)->(0,0) D0|(0,0)->(0,0) E0|(0,0)->(0,0) F0|(8,9)->(8,9) G2|(9,8)->(9,8) H0|(9,9)->(9,9) I0|(9,9)->(9,9) J0|(9,9)->(9,9)]<>{} --- 742
  [A4|(1,0)->(2,0) B3|(0,1)->(1,1) C2|(0,0)->(0,1) D0|(0,0)->(0,0) E0|(0,0)->(0,0) F4|(8,9)->(7,9) G1|(9,8)->(9,8) H2|(9,9)->(8,9) I0|(9,9)->(9,9) J0|(9,9)->(9,9)]<>{} --- 727
  [A3|(2,0)->(2,0) B2|(1,1)->(1,1) C1|(0,1)->(0,1) D1|(0,0)->(1,0) E0|(0,0)->(0,0) F3|(7,9)->(7,9) G0|(9,8)->(9,8) H1|(8,9)->(8,9) I0|(9,9)->(9,9) J0|(9,9)->(9,9)]<>{} --- 735
  [A2|(2,0)->(2,0) B1|(1,1)->(1,1) C0|(0,1)->(0,1) D0|(1,0)->(1,0) E0|(0,0)->(0,0) F2|(7,9)->(7,9) G3|(9,8)->(9,7) H0|(8,9)->(8,9) I1|(9,9)->(9,8) J0|(9,9)->(9,9)]<>{} --- 737
  [A1|(2,0)->(2,0) B0|(1,1)->(1,1) C2|(0,1)->(0,2) D0|(1,0)->(1,0) E0|(0,0)->(0,1) F1|(7,9)->(7,9) G2|(9,7)->(9,7) H2|(8,9)->(8,8) I0|(9,8)->(9,8) J0|(9,9)->(8,9)]<>{} --- 737
  [A0|(2,0)->(2,0) B3|(1,1)->(2,1) C1|(0,2)->(0,2) D1|(1,0)->(1,1) E0|(0,1)->(0,1) F0|(7,9)->(7,9) G1|(9,7)->(9,7) H1|(8,8)->(8,8) I0|(9,8)->(9,8) J0|(8,9)->(8,9)]<>{} --- 740
  [A4|(2,0)->(3,0) B2|(2,1)->(2,1) C0|(0,2)->(0,2) D0|(1,1)->(1,1) E0|(0,1)->(0,1) F4|(7,9)->(6,9) G0|(9,7)->(9,7) H0|(8,8)->(8,8) I0|(9,8)->(9,8) J0|(8,9)->(7,9)]<>{} --- 736
  [A3|(3,0)->(3,0) B1|(2,1)->(2,1) C2|(0,2)->(0,3) D1|(1,1)->(1,2) E0|(0,1)->(1,1) F3|(6,9)->(6,9) G3|(9,7)->(8,7) H0|(8,8)->(8,8) I1|(9,8)->(9,7) J0|(7,9)->(7,8)]<>{} --- 730
  [A2|(3,0)->(3,0) B0|(2,1)->(2,1) C1|(0,3)->(0,3) D0|(1,2)->(1,2) E0|(1,1)->(1,1) F2|(6,9)->(6,9) G2|(8,7)->(8,7) H2|(8,8)->(7,8) I0|(9,7)->(9,7) J0|(7,8)->(6,8)]<>{} --- 735
  [A1|(3,0)->(3,0) B3|(2,1)->(2,2) C0|(0,3)->(0,3) D0|(1,2)->(1,2) E0|(1,1)->(2,1) F1|(6,9)->(6,9) G1|(8,7)->(8,7) H1|(7,8)->(7,8) I0|(9,7)->(9,7) J0|(6,8)->(5,8)]<>{} --- 737
  [A0|(3,0)->(3,0) B2|(2,2)->(2,2) C2|(0,3)->(0,4) D1|(1,2)->(1,3) E0|(2,1)->(3,1) F0|(6,9)->(6,9) G0|(8,7)->(8,7) H0|(7,8)->(7,8) I0|(9,7)->(9,7) J0|(5,8)->(4,8)]<>{} --- 738
  [A4|(3,0)->(4,0) B1|(2,2)->(2,2) C1|(0,4)->(0,4) D0|(1,3)->(1,3) E0|(3,1)->(4,1) F4|(6,9)->(5,9) G3|(8,7)->(7,7) H2|(7,8)->(6,8) I0|(9,7)->(9,7) J0|(4,8)->(4,7)]<>{} --- 725
  [A3|(4,0)->(4,0) B0|(2,2)->(2,2) C0|(0,4)->(0,4) D1|(1,3)->(1,4) E0|(4,1)->(4,2) F3|(5,9)->(5,9) G2|(7,7)->(7,7) H1|(6,8)->(6,8) I1|(9,7)->(8,7) J0|(4,7)->(4,6)]<>{} --- 727
  [A2|(4,0)->(4,0) B0|(2,2)->(2,2) C2|(0,4)->(0,5) D0|(1,4)->(1,4) E0|(4,2)->(4,3) F2|(5,9)->(5,9) G1|(7,7)->(7,7) H0|(6,8)->(6,8) I0|(8,7)->(8,7) J0|(4,6)->(4,5)]<>{} --- 731
  [A1|(4,0)->(4,0) B3|(2,2)->(2,3) C1|(0,5)->(0,5) D1|(1,4)->(1,5) E0|(4,3)->(5,3) F1|(5,9)->(5,9) G0|(7,7)->(7,7) H2|(6,8)->(5,8) I0|(8,7)->(8,7) J0|(4,5)->(3,5)]<>{} --- 727
  [A0|(4,0)->(4,0) B2|(2,3)->(2,3) C0|(0,5)->(0,5) D0|(1,5)->(1,5) E0|(5,3)->(5,4) F0|(5,9)->(5,9) G3|(7,7)->(6,7) H1|(5,8)->(5,8) I1|(8,7)->(7,7) J0|(3,5)->(2,5)]<>{} --- 727
  [A4|(4,0)->(4,1) B1|(2,3)->(2,3) C2|(0,5)->(1,5) D1|(1,5)->(1,6) E0|(5,4)->(6,4) F4|(5,9)->(4,9) G2|(6,7)->(6,7) H0|(5,8)->(5,8) I0|(7,7)->(7,7) J0|(2,5)->(2,4)]<>{} --- 716
  [A3|(4,1)->(4,1) B0|(2,3)->(2,3) C1|(1,5)->(1,5) D0|(1,6)->(1,6) E0|(6,4)->(6,5) F3|(4,9)->(4,9) G1|(6,7)->(6,7) H2|(5,8)->(4,8) I0|(7,7)->(7,7) J0|(2,4)->(1,4)]<>{} --- 720
  [A2|(4,1)->(4,1) B3|(2,3)->(2,4) C0|(1,5)->(1,5) D1|(1,6)->(2,6) E0|(6,5)->(6,6) F2|(4,9)->(4,9) G0|(6,7)->(6,7) H1|(4,8)->(4,8) I0|(7,7)->(7,7) J0|(1,4)->(1,3)]<>{} --- 720
  [A1|(4,1)->(4,1) B2|(2,4)->(2,4) C2|(1,5)->(2,5) D0|(2,6)->(2,6) E0|(6,6)->(6,5) F1|(4,9)->(4,9) G3|(6,7)->(6,6) H0|(4,8)->(4,8) I1|(7,7)->(6,7) J0|(1,3)->(1,2)]<>{} --- 718
  [A0|(4,1)->(4,1) B1|(2,4)->(2,4) C1|(2,5)->(2,5) D1|(2,6)->(2,7) E0|(6,5)->(6,4) F0|(4,9)->(4,9) G2|(6,6)->(6,6) H2|(4,8)->(4,7) I0|(6,7)->(6,7) J0|(1,2)->(0,2)]<>{} --- 722
  [A4|(4,1)->(4,2) B0|(2,4)->(2,4) C0|(2,5)->(2,5) D0|(2,7)->(2,7) E0|(6,4)->(5,4) F4|(4,9)->(3,9) G1|(6,6)->(6,6) H1|(4,7)->(4,7) I0|(6,7)->(6,7) J0|(0,2)->(0,1)]<>{} --- 720
  [A3|(4,2)->(4,2) B3|(2,4)->(2,5) C2|(2,5)->(2,6) D1|(2,7)->(2,8) E0|(5,4)->(5,3) F3|(3,9)->(3,9) G0|(6,6)->(6,6) H0|(4,7)->(4,7) I0|(6,7)->(6,7) J0|(0,1)->(0,0)]<>{} --- 718
  [A2|(4,2)->(4,2) B2|(2,5)->(2,5) C1|(2,6)->(2,6) D0|(2,8)->(2,8) E0|(5,3)->(5,4) F2|(3,9)->(3,9) G0|(6,6)->(6,6) H2|(4,7)->(4,6) I0|(6,7)->(6,7)]<>{} --- 722
  [A1|(4,2)->(4,2) B1|(2,5)->(2,5) C0|(2,6)->(2,6) D1|(2,8)->(3,8) E0|(5,4)->(6,4) F1|(3,9)->(3,9) G3|(6,6)->(6,5) H1|(4,6)->(4,6) I1|(6,7)->(6,6)]<>{} --- 720
  [A0|(4,2)->(4,2) B0|(2,5)->(2,5) C2|(2,6)->(2,7) D0|(3,8)->(3,8) E0|(6,4)->(6,4) F0|(3,9)->(3,9) G2|(6,5)->(6,5) H0|(4,6)->(4,6) I0|(6,6)->(6,6)]<>{} --- 725
  [A4|(4,2)->(4,3) B3|(2,5)->(2,6) C1|(2,7)->(2,7) D1|(3,8)->(4,8) E0|(6,4)->(5,4) F4|(3,9)->(2,9) G1|(6,5)->(6,5) H2|(4,6)->(4,5) I0|(6,6)->(6,6)]<>{} --- 712
  [A3|(4,3)->(4,3) B2|(2,6)->(2,6) C0|(2,7)->(2,7) D0|(4,8)->(4,8) E0|(5,4)->(6,4) F3|(2,9)->(2,9) G0|(6,5)->(6,5) H1|(4,5)->(4,5) I0|(6,6)->(6,6)]<>{} --- 720
  [A2|(4,3)->(4,3) B1|(2,6)->(2,6) C2|(2,7)->(2,8) D1|(4,8)->(5,8) E0|(6,4)->(5,4) F2|(2,9)->(2,9) G3|(6,5)->(6,4) H0|(4,5)->(4,5) I1|(6,6)->(6,5)]<>{} --- 716
  [A1|(4,3)->(4,3) B0|(2,6)->(2,6) C1|(2,8)->(2,8) D0|(5,8)->(5,8) E0|(5,4)->(5,4) F1|(2,9)->(2,9) G2|(6,4)->(6,4) H2|(4,5)->(3,5) I0|(6,5)->(6,5)]<>{} --- 721
  [A0|(4,3)->(4,3) B3|(2,6)->(2,7) C0|(2,8)->(2,8) D1|(5,8)->(6,8) E0|(5,4)->(5,4) F0|(2,9)->(2,9) G1|(6,4)->(6,4) H1|(3,5)->(3,5) I0|(6,5)->(6,5)]<>{} --- 723
  [A4|(4,3)->(5,3) B2|(2,7)->(2,7) C2|(2,8)->(3,8) D0|(6,8)->(6,8) E0|(5,4)->(4,4) F4|(2,9)->(2,8) G0|(6,4)->(6,4) H0|(3,5)->(3,5) I0|(6,5)->(6,5)]<>{} --- 717
  [A3|(5,3)->(5,3) B1|(2,7)->(2,7) C1|(3,8)->(3,8) D1|(6,8)->(7,8) E0|(4,4)->(4,5) F3|(2,8)->(2,8) G3|(6,4)->(5,4) H2|(3,5)->(2,5) I0|(6,5)->(6,5)]<>{} --- 716
  [A2|(5,3)->(5,3) B0|(2,7)->(2,7) C0|(3,8)->(3,8) D0|(7,8)->(7,8) E0|(4,5)->(4,6) F2|(2,8)->(2,8) G2|(5,4)->(5,4) H1|(2,5)->(2,5) I1|(6,5)->(6,4)]<>{} --- 719
  [A1|(5,3)->(5,3) B0|(2,7)->(2,7) C2|(3,8)->(3,9) D1|(7,8)->(7,9) E0|(4,6)->(4,7) F1|(2,8)->(2,8) G1|(5,4)->(5,4) H0|(2,5)->(2,5) I0|(6,4)->(6,4)]<>{} --- 720
  [A0|(5,3)->(5,3) B3|(2,7)->(3,7) C1|(3,9)->(3,9) D0|(7,9)->(7,9) E0|(4,7)->(4,8) F0|(2,8)->(2,8) G0|(5,4)->(5,4) H2|(2,5)->(1,5) I0|(6,4)->(6,4)]<>{} --- 719
  [A4|(5,3)->(5,4) B2|(3,7)->(3,7) C0|(3,9)->(3,9) D1|(7,9)->(8,9) E0|(4,8)->(5,8) F4|(2,8)->(2,7) G3|(5,4)->(4,4) H1|(1,5)->(1,5) I0|(6,4)->(6,4)]<>{} --- 707
  [A3|(5,4)->(5,4) B1|(3,7)->(3,7) C2|(3,9)->(4,9) D0|(8,9)->(8,9) E0|(5,8)->(6,8) F3|(2,7)->(2,7) G2|(4,4)->(4,4) H0|(1,5)->(1,5) I0|(6,4)->(6,4)]<>{} --- 711
  [A2|(5,4)->(5,4) B0|(3,7)->(3,7) C1|(4,9)->(4,9) D1|(8,9)->(9,9) E0|(6,8)->(7,8) F2|(2,7)->(2,7) G1|(4,4)->(4,4) H2|(1,5)->(1,4) I0|(6,4)->(6,4)]<>{} --- 712
  [A1|(5,4)->(5,4) B3|(3,7)->(4,7) C0|(4,9)->(4,9) E0|(7,8)->(8,8) F1|(2,7)->(2,7) G0|(4,4)->(4,4) H1|(1,4)->(1,4) I0|(6,4)->(6,4)]<>{} --- 714
  [A0|(5,4)->(5,4) B2|(4,7)->(4,7) C2|(4,9)->(5,9) E0|(8,8)->(9,8) F0|(2,7)->(2,7) G3|(4,4)->(4,3) H0|(1,4)->(1,4) I0|(6,4)->(6,4)]<>{} --- 712
  [A4|(5,4)->(6,4) B1|(4,7)->(4,7) C1|(5,9)->(5,9) E0|(9,8)->(9,9) F4|(2,7)->(2,6) G2|(4,3)->(4,3) H2|(1,4)->(1,3) I1|(6,4)->(6,5)]<>{} --- 705
  [A3|(6,4)->(6,4) B0|(4,7)->(4,7) C0|(5,9)->(5,9) F3|(2,6)->(2,6) G1|(4,3)->(4,3) H1|(1,3)->(1,3) I0|(6,5)->(6,5)]<>{} --- 712
  [A2|(6,4)->(6,4) B3|(4,7)->(4,8) C2|(5,9)->(6,9) F2|(2,6)->(2,6) G0|(4,3)->(4,3) H0|(1,3)->(1,3) I0|(6,5)->(6,5)]<>{} --- 710
  [A1|(6,4)->(6,4) B2|(4,8)->(4,8) C1|(6,9)->(6,9) F1|(2,6)->(2,6) G3|(4,3)->(4,2) H2|(1,3)->(1,2) I0|(6,5)->(6,5)]<>{} --- 709
  [A0|(6,4)->(6,4) B1|(4,8)->(4,8) C0|(6,9)->(6,9) F0|(2,6)->(2,6) G2|(4,2)->(4,2) H1|(1,2)->(1,2) I0|(6,5)->(6,5)]<>{} --- 716
  [A4|(6,4)->(6,5) B0|(4,8)->(4,8) C2|(6,9)->(7,9) F4|(2,6)->(1,6) G1|(4,2)->(4,2) H0|(1,2)->(1,2) I1|(6,5)->(6,6)]<>{} --- 709
  [A3|(6,5)->(6,5) B3|(4,8)->(5,8) C1|(7,9)->(7,9) F3|(1,6)->(1,6) G0|(4,2)->(4,2) H2|(1,2)->(0,2) I0|(6,6)->(6,6)]<>{} --- 707
  [A2|(6,5)->(6,5) B2|(5,8)->(5,8) C0|(7,9)->(7,9) F2|(1,6)->(1,6) G0|(4,2)->(4,2) H1|(0,2)->(0,2) I0|(6,6)->(6,6)]<>{} --- 714
  [A1|(6,5)->(6,5) B1|(5,8)->(5,8) C2|(7,9)->(8,9) F1|(1,6)->(1,6) G3|(4,2)->(4,1) H0|(0,2)->(0,2) I0|(6,6)->(6,6)]<>{} --- 712
  [A0|(6,5)->(6,5) B0|(5,8)->(5,8) C1|(8,9)->(8,9) F0|(1,6)->(1,6) G2|(4,1)->(4,1) H2|(0,2)->(0,1) I0|(6,6)->(6,6)]<>{} --- 716
  [A4|(6,5)->(6,6) B3|(5,8)->(6,8) C0|(8,9)->(8,9) F4|(1,6)->(1,5) G1|(4,1)->(4,1) H1|(0,1)->(0,1) I1|(6,6)->(6,7)]<>{} --- 708
  [A3|(6,6)->(6,6) B2|(6,8)->(6,8) C2|(8,9)->(9,9) F3|(1,5)->(1,5) G0|(4,1)->(4,1) H0|(0,1)->(0,1) I0|(6,7)->(6,7)]<>{} --- 711
  [A2|(6,6)->(6,6) B1|(6,8)->(6,8) F2|(1,5)->(1,5) G3|(4,1)->(3,1) H2|(0,1)->(0,0) I0|(6,7)->(6,7)]<>{} --- 708
  [A1|(6,6)->(6,6) B0|(6,8)->(6,8) F1|(1,5)->(1,5) G2|(3,1)->(3,1) I0|(6,7)->(6,7)]<>{} --- 713
  [A0|(6,6)->(6,6) B3|(6,8)->(7,8) F0|(1,5)->(1,5) G1|(3,1)->(3,1) I0|(6,7)->(6,7)]<>{} --- 713
  [A4|(6,6)->(6,7) B2|(7,8)->(7,8) F4|(1,5)->(1,4) G0|(3,1)->(3,1) I1|(6,7)->(6,8)]<>{} --- 708
  [A3|(6,7)->(6,7) B1|(7,8)->(7,8) F3|(1,4)->(1,4) G3|(3,1)->(2,1) I0|(6,8)->(6,8)]<>{} --- 708
  [A2|(6,7)->(6,7) B0|(7,8)->(7,8) F2|(1,4)->(1,4) G2|(2,1)->(2,1) I1|(6,8)->(5,8)]<>{} --- 711
  [A1|(6,7)->(6,7) B3|(7,8)->(8,8) F1|(1,4)->(1,4) G1|(2,1)->(2,1) I0|(5,8)->(5,8)]<>{} --- 711
  [A0|(6,7)->(6,7) B2|(8,8)->(8,8) F0|(1,4)->(1,4) G0|(2,1)->(2,1) I1|(5,8)->(4,8)]<>{} --- 713
  [A4|(6,7)->(7,7) B1|(8,8)->(8,8) F4|(1,4)->(1,3) G3|(2,1)->(2,0) I0|(4,8)->(4,8)]<>{} --- 701
  [A3|(7,7)->(7,7) B0|(8,8)->(8,8) F3|(1,3)->(1,3) G2|(2,0)->(2,0) I1|(4,8)->(4,7)]<>{} --- 703
  [A2|(7,7)->(7,7) B3|(8,8)->(9,8) F2|(1,3)->(1,3) G1|(2,0)->(2,0) I0|(4,7)->(4,7)]<>{} --- 703
  [A1|(7,7)->(7,7) B2|(9,8)->(9,8) F1|(1,3)->(1,3) G0|(2,0)->(2,0) I1|(4,7)->(4,6)]<>{} --- 706
  [A0|(7,7)->(7,7) B1|(9,8)->(9,8) F0|(1,3)->(1,3) G3|(2,0)->(1,0) I0|(4,6)->(4,6)]<>{} --- 706
  [A4|(7,7)->(8,7) B0|(9,8)->(9,8) F4|(1,3)->(1,2) G2|(1,0)->(1,0) I1|(4,6)->(4,5)]<>{} --- 696
  [A3|(8,7)->(8,7) B3|(9,8)->(9,9) F3|(1,2)->(1,2) G1|(1,0)->(1,0) I0|(4,5)->(4,5)]<>{} --- 696
  [A2|(8,7)->(8,7) F2|(1,2)->(1,2) G0|(1,0)->(1,0) I1|(4,5)->(3,5)]<>{} --- 697
  [A1|(8,7)->(8,7) F1|(1,2)->(1,2) G3|(1,0)->(0,0) I0|(3,5)->(3,5)]<>{} --- 696
  [A0|(8,7)->(8,7) F0|(1,2)->(1,2) I1|(3,5)->(2,5)]<>{} --- 697
  [A4|(8,7)->(9,7) F4|(1,2)->(1,1) I0|(2,5)->(2,5)]<>{} --- 687
  [A3|(9,7)->(9,7) F3|(1,1)->(1,1) I1|(2,5)->(2,4)]<>{} --- 688
  [A2|(9,7)->(9,7) F2|(1,1)->(1,1) I0|(2,4)->(2,4)]<>{} --- 691
  [A1|(9,7)->(9,7) F1|(1,1)->(1,1) I1|(2,4)->(1,4)]<>{} --- 691
  [A0|(9,7)->(9,7) F0|(1,1)->(1,1) I0|(1,4)->(1,4)]<>{} --- 694
  [A4|(9,7)->(9,8) F4|(1,1)->(0,1) I1|(1,4)->(1,3)]<>{} --- 682
  [A3|(9,8)->(9,8) F3|(0,1)->(0,1) I0|(1,3)->(1,3)]<>{} --- 685
  [A2|(9,8)->(9,8) F2|(0,1)->(0,1) I1|(1,3)->(1,2)]<>{} --- 686
  [A1|(9,8)->(9,8) F1|(0,1)->(0,1) I0|(1,2)->(1,2)]<>{} --- 689
  [A0|(9,8)->(9,8) F0|(0,1)->(0,1) I1|(1,2)->(0,2)]<>{} --- 689
  [A4|(9,8)->(9,9) F4|(0,1)->(0,0) I0|(0,2)->(0,2)]<>{} --- 680
  [I1|(0,2)->(0,1)]<>{} --- 678
  [I0|(0,1)->(0,1)]<>{} --- 679
  [I1|(0,1)->(0,0)]<>{} --- 678
    nodes opened:981032
    nodes closed:765482
    nodes priority increased:0
    nodes reopened:0
  */
}


