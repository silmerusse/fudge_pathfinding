#include <vector>
#include <iostream>
#include <gtest/gtest.h>
#include "util/time_util.h"
#include "load_matrix.h"
#include "rra.h"

TEST(RRA, search) {
  std::vector<int> matrix = fudge::load_matrix<int>(
      "../data/matrix_10x10_plain.txt");

  fudge::RRA<int> rra(matrix, 10, 10);
  int a = rra.search(fudge::Coord(0, 0), fudge::Coord(5, 5),
                     fudge::GridMap<int>::manhattan_distance);
  ASSERT_EQ(10, a);

  int b = rra.search(fudge::Coord(0, 0), fudge::Coord(9, 9),
                     fudge::GridMap<int>::manhattan_distance);
  ASSERT_EQ(18, b);

  int c = rra.search(fudge::Coord(0, 0), fudge::Coord(2, 2),
                     fudge::GridMap<int>::manhattan_distance);
  ASSERT_EQ(4, c);
}
