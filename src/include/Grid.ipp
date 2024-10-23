#include <Grid.hpp>
#include <iostream>
#include <limits>

namespace war {

template <class T>
Grid<T>::Grid(const point_t &min, const point_t &max, const index_t &dimensions)
    : min(min), max(max), dimensions(dimensions), size(max - min) {}

template <class T>
Grid<T>::Grid(const point_t &min, const point_t &max,
              const index_t &&dimensions)
    : min(min), max(max), dimensions(dimensions), size(max - min) {}

template <class T>
typename Grid<T>::bucket_t &Grid<T>::operator[](const index_t &i) const {
  const size_t i1d =
      i[0] * dimensions[0] * dimensions[1] + i[1] * dimensions[1] + i[2];
  return data[i1d];
}
template <class T>
bool Grid<T>::getIndex(const Ray &ray, index_t &result) const {
  scalar_t t;
  if (rayHit(ray, t)) {
    result = worldToGrid(ray.at(t));
    return true;
  }
  return false;
}
template <class T> bool Grid<T>::rayHit(const Ray &ray, scalar_t &t) const {
  // check if ray origin is inside the grid
  if (glm::all(glm::greaterThanEqual(ray.O, min)) &&
      glm::all(glm::lessThanEqual(ray.O, max))) {
    t = 0;
    return true;
  }
  // test for ray vs grid AABB intersection
  // just gotta eval the closest hit since the case of the origin beign inside
  // the box is already covered
  vec_t tmin = (min - ray.O) / ray.D;
  vec_t tmax = (max - ray.O) / ray.D;
  vec_t opt = glm::min(tmin, vec_t(tmax.y, tmax.z, tmax.x));
  t = glm::max(glm::max(opt.x, opt.y), opt.z);
  if (t >= 0) {
    return true;
  }
  return false;
}

template <class T>
typename Grid<T>::Iterator Grid<T>::begin(const Ray &ray) const {
  return Iterator(const_cast<Grid<T> *>(this), ray);
}

template <class T> typename Grid<T>::Iterator Grid<T>::end() const {
  return Iterator(nullptr, index_t(MAX_INDEX));
}

template <class T>
typename Grid<T>::index_t Grid<T>::worldToGrid(const point_t &p) const {
  if (glm::any(glm::lessThan(p, min)) ||
      glm::any(glm::greaterThan(p, max + EPSILON * 5)))
    return {MAX_INDEX, MAX_INDEX, MAX_INDEX};

  const vec_t wi = glm::clamp(
      vec_t(0),
      vec_t(dimensions[0], dimensions[1], dimensions[2]) * (p - min) / size,
      vec_t(dimensions[0] - 1, dimensions[1] - 1, dimensions[2] - 1));
  return {size_t(wi.x), size_t(wi.y), size_t(wi.z)};
}
template <class T>
Grid<T>::Iterator::Iterator(Grid<T> *g, const index_t &index)
    : grid(g), current(index) {}

template <class T>
Grid<T>::Iterator::Iterator(Grid<T> *g, const Ray &ray) : grid(g) {
  scalar_t t;
  if (!g) {
    current = {MAX_INDEX, MAX_INDEX, MAX_INDEX};
    return;
  }
  if (!g->rayHit(ray, t)) {
    g = nullptr;
    current = {MAX_INDEX, MAX_INDEX, MAX_INDEX};
    return;
  }
  const point_t origin = ray.at(t);
  current = g->worldToGrid(origin);
  const vec_t floor = glm::floor(origin);
  const vec_t ceil = glm::ceil(origin);

  step = glm::sign(ray.D);
  const vec_t abs = glm::abs(step);
  const vec_t frac =
      (vec_t(1) - abs) * (ceil - origin) + abs * (origin - floor);

  tdelta = (grid->size / vec_t(grid->dimensions)) / ray.D;
  tlimit = origin + tdelta * frac;
}

template <class T> typename Grid<T>::Iterator &Grid<T>::Iterator::operator++() {
  std::cout << "IN OPERATOR PLUS PLUS START: grid = " << grid << " - current = {"
            << current[0] << ", " << current[1] << ", " << current[2] << "}"
            << std::endl;
  if (!grid)
    return *this;
  const scalar_t minT = glm::min(glm::min(tlimit.x, tlimit.y), tlimit.z);
  for (size_t i = 0; i < DIM; i++) {
    if (tlimit[i] != minT)
      continue;
    current[i] += step[i];
    tlimit[i] += tdelta[i];
  }
    if (glm::any(glm::greaterThanEqual(current, grid->dimensions))) {
      grid = nullptr;
      current = index_t(MAX_INDEX);
    }

  std::cout << "IN OPERATOR PLUS PLUS END: grid = " << grid << " - current = {"
            << current[0] << ", " << current[1] << ", " << current[2] << "}"
            << std::endl;
  return *this;
}

template <class T>
typename Grid<T>::bucket_t &Grid<T>::Iterator::operator*() const {
  return (*grid)[current];
}

template <class T>
typename Grid<T>::bucket_t *Grid<T>::Iterator::operator->() const {
  return &(*grid)[current];
}

template <class T>
bool Grid<T>::Iterator::operator==(const Iterator &other) const {
  return this->grid == other.grid && this->current == other.current;
}
template <class T>
bool Grid<T>::Iterator::operator!=(const Iterator &other) const {
  return !(*this == other);
}

template <class T>
const typename Grid<T>::index_t Grid<T>::Iterator::getIndex() const {
  return current;
}
}; // namespace war
