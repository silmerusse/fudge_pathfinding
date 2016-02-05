#ifndef FUDGE_VERTEX_MATRIX_H_
#define FUDGE_VERTEX_MATRIX_H_

#include <algorithm>
#include <string>
#include <sstream>

namespace fudge {

using Coord = std::pair<int,int>;

template<typename WeightType>
class VertexMatrix {
public:
  VertexMatrix(int width, int height, const std::vector<WeightType> &matrix):
       width_(width), height_(height), matrix_(matrix) {}
  virtual ~VertexMatrix() {}

public:
  int width_ = 0;
  int height_ = 0;

public:
  const WeightType weight(Coord coord) const {
    return matrix_[coord.second * width_ + coord.first];
  }

  bool is_off(Coord coord) const {
    return (coord.first < 0 || coord.first >= width_ ||
        coord.second < 0 || coord.second >= height_);
  }

  bool is_passable(Coord coord,
      WeightType threhold = std::numeric_limits<WeightType>::max()) const {
    return (!is_off(coord) &&
        weight(coord) >= 0 && weight(coord) <= threhold);
  }

  const std::string to_string() const {
    std::ostringstream ss;
    ss << "Vertex Matrix:" << std::endl;

    for (int i=0; i<height_; i++) {
      for (int j=0; j<width_; j++) {
        char c = ' ';
        int v = static_cast<int>(weight(Coord(j,i)));
        if (v < 0)
          c = 'x';
        else if (v == 1)
          c = '-';
        else if (v > 9)
          c = '+';
        else
          c = v + '0';
        ss << c;
      }
      ss << std::endl;
    }
    return ss.str();
  }

private:
  const std::vector<WeightType> &matrix_;
};

}

#endif /* FUDGE_VERTEX_MATRIX_H_ */
