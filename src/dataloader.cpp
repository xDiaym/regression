#include <fstream>
#include "dataloader.hpp"


Dataloader::LoadResult CSVLoader::load(std::vector<Vec2> &dataset) {
  std::ifstream in(path_);
  if (in.bad())
    return LoadResult::kFileError;
  while (!in.eof()) {
    double x1, x2; char comma_;
    in >> x1 >> comma_ >> x2;
    if (in.bad())
      return LoadResult::kFormatError;
    dataset.emplace_back(x1, x2);
  }
  return Dataloader::kOk;
}
