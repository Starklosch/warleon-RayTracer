#pragma once
#include <AABB.hpp>
#include <Ray.hpp>
#include <array>
#include <common.hpp>
#include <unordered_map>
#include <utility>
#include <vector>

namespace war {
template <class T> class Grid {
public:
  typedef std::vector<T> bucket_t;
  typedef glm::tvec3<size_t> index_t;
  const point_t min, max;
  const index_t dimensions;
  const vec_t size, boxSize;

  Grid(const point_t &min, const point_t &max, const index_t &dimensions);
  // Grid(const point_t &min, const point_t &max, const index_t &&dimensions);
  bucket_t &operator[](const index_t &i) const;
  index_t worldToGrid(const point_t &p) const;
  bool rayHit(const Ray &ray, scalar_t &t) const;
  bool getIndex(const Ray &ray, index_t &result) const;

  aabb_t getAABB(const index_t &i);

  class Iterator {
  public:
    mutable Grid<T> *grid;
    mutable index_t current;
    ivec_t step;
    vec_t tdelta;
    mutable vec_t tlimit;

    Iterator(Grid<T> *g, const Ray &ray);
    Iterator(Grid<T> *g, const index_t &index);
    Iterator &operator++();
    bucket_t &operator*() const;
    bucket_t *operator->() const;
    bool operator==(const Iterator &other) const;
    bool operator!=(const Iterator &other) const;
    const index_t getIndex() const;
  };
  Iterator begin(const Ray &ray) const;
  Iterator end() const;

  friend Iterator;

private:
  mutable std::unordered_map<size_t, bucket_t> data;
};
}; // namespace war
#include <Grid.ipp>
