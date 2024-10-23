#pragma once
#include <Ray.hpp>
#include <array>
#include <common.hpp>
#include <unordered_map>
#include <vector>

namespace war {
template <class T> class Grid {
  typedef std::vector<T> bucket_t;
  typedef std::array<size_t, 3> index_t;

  std::unordered_map<size_t, bucket_t> data;

public:
  const point_t min, max;
  const vec_t size;
  const index_t dimensions;
  const static bucket_t defaulBucket;

  Grid(const point_t &min, const point_t &max, const index_t &dimensions);
  Grid(const point_t &min, const point_t &max, const index_t &&dimensions);
  bucket_t &operator[](const index_t &i);
  const bucket_t &operator[](const index_t &i) const;
  index_t worldToGrid(const point_t &p);

  class Iterator {
    Grid<T> *grid;
    index_t current;
    point_t acc;
    vec_t diff;
    ivec_t signs;

  public:
    Iterator(Grid<T> *g, const index_t &curr);
    Iterator &operator++(int);
    bucket_t &operator*();
    bucket_t *operator->();
    bool operator==(const Iterator &other) const;
    bool operator!=(const Iterator &other) const;
  };
  Iterator begin(const Ray &ray);
  Iterator end();
};
}; // namespace war
#include <Grid.ipp>
