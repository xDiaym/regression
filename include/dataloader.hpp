#pragma once
#include <utility>
#include <vector>
#include <filesystem>
#include <linalg.hpp>

class Dataloader {
public:
  enum LoadResult { kOk, kFileError, kFormatError };

  [[nodiscard]]
  virtual LoadResult load(std::vector<Vec2>& dataset) = 0;
};

class CSVLoader : public Dataloader {
public:
  explicit CSVLoader(std::filesystem::path  path)
      : path_(std::move(path)) {}

  [[nodiscard]] auto load(std::vector<Vec2> &dataset) -> LoadResult final;

private:
  std::filesystem::path path_;
};
