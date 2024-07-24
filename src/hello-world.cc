#include <ctime>
#include <string>
#include <iostream>
#include "common/math/wykobi/wykobi.hpp"

std::string get_greet(const std::string& who) {
  return "Hello " + who;
}

void print_localtime() {
  std::time_t result = std::time(nullptr);
  std::cout << std::asctime(std::localtime(&result));
}

int main(int argc, char** argv) {
  wykobi::point2d<double> pt1 = wykobi::make_point(12.11111, 23.33333);
  wykobi::point2d<double> pt2 = wykobi::make_point(12.11111, 22.33333);
  std::cout << wykobi::distance(pt1, pt2) << "\n";

  std::string who = "world";
  if (argc > 1) {
    who = argv[1];
  }
  std::cout << get_greet(who) << std::endl;
  print_localtime();
  return 0;
}
