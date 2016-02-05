#include <fstream>
#include <gtest/gtest.h>
#include "jump_point_map.h"
#include "astar_search.h"
#include "load_matrix.h"
#include "util/time_util.h"

// Helper to print and return result.
const std::string print_result(const std::vector<fudge::Coord> &path,
                               const fudge::JumpPointMap<double> &map) {
  std::cout << "--------------------\n";
  std::ostringstream ss;
  for (auto i = path.rbegin(); i != path.rend(); ++i)
    ss << map.node(*i)->to_string() << std::endl;
  ss << map.to_string() << std::endl;
  std::cout << ss.str();
  return ss.str();
}


TEST(JumpPointMap, search_10x10) {
  std::vector<double> matrix = fudge::load_matrix<double>(
      "../data/matrix_10x10_plain.txt");

  PREPARE_TIMER
  START_TIMER
    fudge::JumpPointMap<double> map(10, 10, matrix);
    map.goal_ = fudge::Coord(9, 9);
    const std::vector<fudge::Coord> path0 = fudge::astar_search(
        map, fudge::Coord(0, 0), fudge::Coord(9, 9), 
        fudge::GridMap<double>::diagonal_distance);

  END_TIMER
  PRINT_TIME_ELAPSED

  std::string result = print_result(path0, map);

  std::string expected = std::string("") +
      "(9,9)<-(0,0) g:12.7287 f:12.7287" + "\n" +
      "  nodes opened:2"                      + "\n" +
      "  nodes closed:2"                      + "\n" +
      "  nodes priority increased:0"           + "\n" +
      "  nodes reopened:0"                     + "\n" +
      ""                                       + "\n" +
      "S         " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "         G" + "\n" +
      "\n";

  ASSERT_EQ(expected, result);

  /*
   * Second search.
   */
  START_TIMER
    fudge::JumpPointMap<double> map2(10, 10, matrix);
    map2.goal_ = fudge::Coord(0, 9);
    const std::vector<fudge::Coord> path1 = fudge::astar_search(
        map2, fudge::Coord(9, 0), fudge::Coord(0, 9),
        fudge::GridMap<double>::duclidean_distance);

  END_TIMER
  PRINT_TIME_ELAPSED

  std::string result2 = print_result(path1, map2);

  std::string expected2 = std::string("") +
      "(0,9)<-(9,0) g:12.7287 f:12.7287" + "\n" +
      "  nodes opened:2"                 + "\n" +
      "  nodes closed:2"                 + "\n" +
      "  nodes priority increased:0"     + "\n" +
      "  nodes reopened:0"               + "\n" +
      ""                                 + "\n" +
      "         S" + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "          " + "\n" +
      "G         " + "\n" +
      "\n";

  ASSERT_EQ(expected2, result2);
}

TEST(JumpPointMap, search_10x10_wall) {
  std::vector<double> matrix2 = fudge::load_matrix<double>(
      "../data/matrix_10x10_wall.txt");

  PREPARE_TIMER
  START_TIMER
    fudge::JumpPointMap<double> map(10, 10, matrix2);
  map.goal_ = fudge::Coord(4, 1);
  const std::vector<fudge::Coord> path = fudge::astar_search(
        map, fudge::Coord(7, 0), fudge::Coord(4, 1),
        fudge::GridMap<double>::diagonal_distance);
  END_TIMER
  PRINT_TIME_ELAPSED

  std::string result = print_result(path, map);

  std::string expected = std::string("") +
      "(8,1)<-(7,0) g:1.4143 f:5.4143" + "\n" +
      "(8,7)<-(8,1) g:7.4143 f:15.0715" + "\n" +
      "(7,8)<-(8,7) g:8.8286 f:17.0715" + "\n" +
      "(5,8)<-(7,8) g:10.8286 f:18.2429" + "\n" +
      "(4,7)<-(5,8) g:12.2429 f:18.2429" + "\n" +
      "(3,7)<-(4,7) g:13.2429 f:19.6572" + "\n" +
      "(0,4)<-(3,7) g:17.4858 f:22.7287" + "\n" +
      "(3,1)<-(0,4) g:21.7287 f:22.7287" + "\n" +
      "(4,1)<-(3,1) g:22.7287 f:22.7287" + "\n" +
      "  nodes opened:14" + "\n" +
      "  nodes closed:14" + "\n" +
      "  nodes priority increased:1" + "\n" +
      "  nodes reopened:0" + "\n" +
      "" + "\n" +
      "     x S  " + "\n" +
      "   @Gx  @ " + "\n" +
      "   xxxx   " + "\n" +
      "   x  xx  " + "\n" +
      "@xxx - x  " + "\n" +
      "   x-x-x  " + "\n" +
      "   x x x  " + "\n" +
      "   @@x-x@ " + "\n" +
      "     @ @  " + "\n" +
      "          " + "\n" +
      "\n";

  ASSERT_EQ(expected, result);
}

TEST(JumpPointMap, search_100x100) {
  std::vector<double> matrix2 = fudge::load_matrix<double>(
      "../data/matrix_100x100.txt");

  PREPARE_TIMER
  START_TIMER
    fudge::JumpPointMap<double> map(100, 100, matrix2);
    map.goal_ = fudge::Coord(99, 99);
    const std::vector<fudge::Coord> path = fudge::astar_search(
        map, fudge::Coord(0, 0), fudge::Coord(99, 99),
        fudge::GridMap<double>::diagonal_distance);
  END_TIMER
  PRINT_TIME_ELAPSED

  std::string result = print_result(path, map);

  ASSERT_EQ(80, path.size());
}
