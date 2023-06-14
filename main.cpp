#include <regression.hpp>
#include <dataloader.hpp>
#include <vector>
#include <iostream>

int main() {
  CSVLoader loader("../small.csv");
  std::vector<Vec2> dataset;
  if (loader.load(dataset) != Dataloader::kOk)
    return -1;
  Regression reg(std::move(dataset));
  auto [a, b] = reg.fit();
  std::cout << a << ' ' << b << std::endl;
}