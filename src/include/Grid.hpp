#pragma once
#include <Ray.hpp>
#include <array>
#include <common.hpp>
#include <unordered_map>
#include <vector>

namespace war {
template <class T> class Grid {
public:
  typedef std::vector<T> bucket_t;
  typedef std::array<size_t, 3> index_t;
  const point_t min, max;
  const vec_t size;
  const index_t dimensions;

  Grid(const point_t &min, const point_t &max, const index_t &dimensions);
  Grid(const point_t &min, const point_t &max, const index_t &&dimensions);
  bucket_t &operator[](const index_t &i) const;
  index_t worldToGrid(const point_t &p) const;

  class Iterator {
    Grid<T> *grid;
    index_t current;
    point_t acc;
    vec_t diff;
    ivec_t signs;

  public:
    Iterator(Grid<T> *g, const index_t &curr);
    Iterator &operator++(int);
    bucket_t &operator*() const;
    bucket_t *operator->() const;
    bool operator==(const Iterator &other) const;
    bool operator!=(const Iterator &other) const;
  };
  Iterator begin(const Ray &ray)const;
  Iterator end()const;
private:
  mutable std::unordered_map<size_t, bucket_t> data;
};
}; // namespace war
#include <Grid.ipp>
