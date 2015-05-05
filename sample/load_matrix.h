#ifndef LOAD_MATRIX_H_
#define LOAD_MATRIX_H_

#include <string>
#include <vector>
#include <fstream>

using namespace std;

template <typename WeightType>
static vector<WeightType> load_matrix(const string &filename) {
  vector<WeightType> v;

  ifstream ifs(filename);
  if(ifs.is_open()) {
    while(ifs.good()) {
      string line;
      getline(ifs, line);
      stringstream ss(line);
      string s;
      while (getline(ss, s, ',' )
          && s.find_first_not_of(" \t\r\n") != s.npos ) {
        stringstream is(s);
        WeightType i;
        is >> i;
        v.push_back(i);
      }
    }
  } else {
    printf("File not found.\n");
  }
  return v;
}


#endif /* LOAD_MATRIX_H_ */
