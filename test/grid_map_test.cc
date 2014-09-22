#include <gtest/gtest.h>
#include "grid_map.h"
#include "astar_search.h"
#include "load_matrix.h"
#include "time_util.h"

// Helper to print and return result.
const string print_result(const vector<Coord> &path,
                          const GridMap<double> &map) {
  std::cout << "--------------------\n";
  ostringstream ss;
  for (auto e : path)
    ss << map.node(e)->to_string() << '\n';
  ss << map.to_string() << '\n';
  std::cout<< ss.str();
  return ss.str();
}

TEST(GridMap, search_10x10) {
  vector<double> matrix = load_matrix<double>("../data/matrix_10x10_plain.txt");

  PREPARE_TIMER
  START_TIMER
    GridMap<double> map(10, 10, matrix);
    vector<Coord> &&path = AStarSearch::search(map, Coord(0, 0), Coord(9, 9),
                                          GridMap<double>::diagonal_distance);
  END_TIMER
  PRINT_TIME_ELAPSED

  string result = print_result(path, map);

  string expected = string("") +
      "(9,9)<-(8,8) g:12.7287 f:12.7287" + "\n" +
      "(8,8)<-(7,7) g:11.3144 f:12.7287" + "\n" +
      "(7,7)<-(6,6) g:9.9001 f:12.7287"  + "\n" +
      "(6,6)<-(5,5) g:8.4858 f:12.7287"  + "\n" +
      "(5,5)<-(4,4) g:7.0715 f:12.7287"  + "\n" +
      "(4,4)<-(3,3) g:5.6572 f:12.7287"  + "\n" +
      "(3,3)<-(2,2) g:4.2429 f:12.7287"  + "\n" +
      "(2,2)<-(1,1) g:2.8286 f:12.7287"  + "\n" +
      "(1,1)<-(0,0) g:1.4143 f:12.7287"  + "\n" +
      "  nodes opened:44"                      + "\n" +
      "  nodes closed:10"                      + "\n" +
      "  nodes priority increased:0"           + "\n" +
      "  nodes reopened:0"                     + "\n" +
      ""                                       + "\n" +
      "Soo       " + "\n" +
      "o@oo      " + "\n" +
      "oo@oo     " + "\n" +
      " oo@oo    " + "\n" +
      "  oo@oo   " + "\n" +
      "   oo@oo  " + "\n" +
      "    oo@oo " + "\n" +
      "     oo@oo" + "\n" +
      "      oo@o" + "\n" +
      "       ooG" + "\n" +
      "\n";

  ASSERT_EQ(expected, result);


  /*
   * Second search.
   */
  START_TIMER
    GridMap<double> map2(10, 10, matrix);
    path = AStarSearch::search(map2, Coord(9, 0), Coord(0, 9),
                               GridMap<double>::duclidean_distance);
  END_TIMER
  PRINT_TIME_ELAPSED

  string result2 = print_result(path, map2);

  string expected2 = string("") +
      "(0,9)<-(1,8) g:12.7287 f:12.7287" + "\n" +
      "(1,8)<-(2,7) g:11.3144 f:12.7286" + "\n" +
      "(2,7)<-(3,6) g:9.9001 f:12.7285"  + "\n" +
      "(3,6)<-(4,5) g:8.4858 f:12.7284"  + "\n" +
      "(4,5)<-(5,4) g:7.0715 f:12.7284"  + "\n" +
      "(5,4)<-(6,3) g:5.6572 f:12.7283"  + "\n" +
      "(6,3)<-(7,2) g:4.2429 f:12.7282"  + "\n" +
      "(7,2)<-(8,1) g:2.8286 f:12.7281"  + "\n" +
      "(8,1)<-(9,0) g:1.4143 f:12.728"   + "\n" +
      "  nodes opened:44"                      + "\n" +
      "  nodes closed:10"                      + "\n" +
      "  nodes priority increased:0"           + "\n" +
      "  nodes reopened:0"                     + "\n" +
      ""                                       + "\n" +
      "       ooS" + "\n" +
      "      oo@o" + "\n" +
      "     oo@oo" + "\n" +
      "    oo@oo " + "\n" +
      "   oo@oo  " + "\n" +
      "  oo@oo   " + "\n" +
      " oo@oo    " + "\n" +
      "oo@oo     " + "\n" +
      "o@oo      " + "\n" +
      "Goo       " + "\n" +
      "\n";


  ASSERT_EQ(expected2, result2);

}


TEST(GridMap, search_100x100) {
  vector<double> matrix = load_matrix<double>("../data/matrix_100x100.txt");

  PREPARE_TIMER
  START_TIMER
    GridMap<double> map(100, 100, matrix);
    vector<Coord> &&path = AStarSearch::search(map,
                                          Coord(0, 0), Coord(99, 99),
                                          GridMap<double>::diagonal_distance);
  END_TIMER
  PRINT_TIME_ELAPSED

  print_result(path, map);

  ASSERT_EQ(135, path.size());

}



