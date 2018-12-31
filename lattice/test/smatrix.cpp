#include <iostream>
#include <Eigen/Dense>

int main() {
  Eigen::Matrix<std::string, Eigen::Dynamic, Eigen::Dynamic> smatrix(3, 3);
  smatrix(0,0) = "c";
  smatrix(1,1) = "a";
  std::cout << smatrix << std::endl;
}

