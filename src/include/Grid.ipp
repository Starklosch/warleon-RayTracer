#include <Grid.hpp>
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
typename Grid<T>::Iterator Grid<T>::begin(const Ray &ray) const {
  // check if ray origin is inside the grid
  const index_t index = worldToGrid(ray.O);
  bool bounded = true;
  for (size_t d = 0; d < index.size(); d++) {
    bounded &= index[d] >= 0 && index[d] < dimensions[d];
  }
  if (bounded) {
    return Iterator(const_cast<Grid<T> *>(this), index);
  }
  // test for ray vs grid AABB intersection
  // just gotta eval the closest hit since the case of the origin beign inside
  // the box is already covered
  vec_t tmin = (min - ray.O) / ray.D;
  vec_t tmax = (max - ray.O) / ray.D;
  vec_t opt = glm::min(tmin, vec_t(tmax.y, tmax.z, tmax.x));
  scalar_t t = glm::max(glm::max(opt.x, opt.y), opt.z);
  if (t >= 0) {
    return Iterator(const_cast<Grid<T> *>(this), worldToGrid(ray.at(t)));
  }
  // return end() if no intersection occurs
  return end();
}

template <class T> typename Grid<T>::Iterator Grid<T>::end() const {
  size_t maxi = std::numeric_limits<size_t>::max();
  return Iterator(nullptr, {maxi, maxi, maxi});
}

template <class T>
typename Grid<T>::index_t Grid<T>::worldToGrid(const point_t &p) const {
  size_t maxi = std::numeric_limits<size_t>::max();
  if (glm::any(glm::lessThan(p, min)) ||
      glm::any(glm::greaterThan(p, max + EPSILON * 5)))
    return {maxi, maxi, maxi};

  const vec_t wi = glm::clamp(
      vec_t(0),
      vec_t(dimensions[0], dimensions[1], dimensions[2]) * (p - min) / size,
      vec_t(dimensions[0] - 1, dimensions[1] - 1, dimensions[2] - 1));
  return {size_t(wi.x), size_t(wi.y), size_t(wi.z)};
}

template <class T>
Grid<T>::Iterator::Iterator(Grid<T> *g, const index_t &curr)
    : grid(g), current(curr) {}

template <class T>
typename Grid<T>::Iterator &Grid<T>::Iterator::operator++(int) {
  // TODO IMPLEMENT
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
