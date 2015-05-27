#include <vector>
#include <iostream>
#include <gtest/gtest.h>
#include "time_util.h"
#include "load_matrix.h"
#include "rra.h"


TEST(RRA, search) {
  std::vector<int> matrix = load_matrix<int>("../data/matrix_10x10_plain.txt");

  RRA<int> rra(matrix, 10, 10);
  int a = rra.search(Coord(0, 0), Coord(5, 5),
      GridMap<int>::manhattan_distance);
  ASSERT_EQ(10, a);

  int b = rra.search(Coord(0, 0), Coord(9, 9),
      GridMap<int>::manhattan_distance);
  ASSERT_EQ(18, b);

  int c = rra.search(Coord(0, 0), Coord(2, 2),
      GridMap<int>::manhattan_distance);
  ASSERT_EQ(4, c);

}
