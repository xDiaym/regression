#include <iostream>
#include <vector>
#include <linalg.hpp>
#include <regression.hpp>
#include <dataloader.hpp>

auto main() -> int {
  CSVLoader loader("../dataset.csv");
  std::vector<Vec2> dataset;
  if (loader.load(dataset) != CSVLoader::kOk) {
    std::cerr << "File error\n";
    return -1;
  }

  Regression r(std::move(dataset));
  auto[a, b] = r.fit();

  std::cout << a << ' ' << b << std::endl;
  return 0;
}
