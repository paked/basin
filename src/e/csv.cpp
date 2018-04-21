#include <e/csv.hpp>

#include <stdio.h>

#include <iostream>

CSV::CSV(std::string f, char delm) : fname(f), delimeter(delm) {}

std::vector<std::vector<std::string> > CSV::getData() {
  std::vector<std::vector<std::string> > dataList;
  std::ifstream file;

  file.open(fname);

  if (!file.is_open()) {
    printf("could not open file!\n");

    return dataList;
  }

  if (!file.good()) {
    printf("file is not good!\n");

    return dataList;
  }

  std::string line = "";

  while(file.good()) {
    getline(file, line);

    std::vector<std::string> vec;

    std::string current;
    for (auto c : line) {
      if (c == delimeter || c == '\n') {
        vec.push_back(current);
        current = "";
      } else {
        current += c;
      }
    }

    dataList.push_back(vec);
  }

  file.close();

  return dataList;
}
