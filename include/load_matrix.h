#ifndef FUDGE_LOAD_MATRIX_H_
#define FUDGE_LOAD_MATRIX_H_

#include <string>
#include <vector>
#include <fstream>
#include "util/log.h"

namespace fudge {

template <typename WeightType>
static std::vector<WeightType> load_matrix(const std::string &filename) {
  std::vector<WeightType> v;

  std::ifstream ifs(filename);
  if(ifs.is_open()) {
    while(ifs.good()) {
      std::string line;
      getline(ifs, line);
      std::istringstream ss(line);
      std::string s;
      while (getline(ss, s, ',' )
          && s.find_first_not_of(" \t\r\n") != s.npos ) {
        std::istringstream is(s);
        WeightType i;
        is >> i;
        v.push_back(i);
      }
    }
  } else {
    ERROR("File not found.\n");
  }
  return v;
}

}

#endif /* FUDGE_LOAD_MATRIX_H_ */
