#include <Grid.hpp>
#include <cassert>
#include <limits>

namespace war {

	template <class T>
	Grid<T>::Grid(const point_t& min, const point_t& max, const index_t& dimensions)
		: min(min), max(max), dimensions(dimensions), size(max - min),
		boxSize((max - min) / vec_t(dimensions)) {
		print("grid MBB : min = [{}, {}, {}], max = [{}, {}, {}]\n", min[0], min[1],
			min[2], max[0], max[1], max[2]);
		print("grid Dimensions = [{}, {}, {}]\n", dimensions[0], dimensions[1],
			dimensions[2]);
	}

	template <class T>
	typename Grid<T>::bucket_t& Grid<T>::operator[](const index_t& i) const {
		assert(i[0] < dimensions[0]);
		assert(i[1] < dimensions[1]);
		assert(i[2] < dimensions[2]);
		const size_t i1d =
			i.z + i.y * dimensions.z + i.x * dimensions.z * dimensions.y;
		return data[i1d];
	}

	template <class T> bool Grid<T>::rayHit(const Ray& ray, scalar_t& t) const {
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
		if (t > EPSILON) {
			return true;
		}
		return false;
	}

	template <class T>
	typename Grid<T>::Iterator Grid<T>::begin(const Ray& ray) const {
		return Iterator(const_cast<Grid<T> *>(this), ray);
	}

	template <class T>
	typename Grid<T>::Iterator Grid<T>::end() const {
		return Iterator(nullptr, index_t(MAX_INDEX));
	}

	template <class T>
	index_t Grid<T>::worldToGrid(const point_t& p) const {
		const vec_t wi = glm::clamp(
			vec_t(0),
			vec_t(dimensions[0], dimensions[1], dimensions[2]) * (p - min) / size,
			vec_t(dimensions[0] - 1, dimensions[1] - 1, dimensions[2] - 1));
		return { size_t(wi.x), size_t(wi.y), size_t(wi.z) };
		// return vec_t(dimensions)*(p-min)/size;
	}
	template <class T> aabb_t Grid<T>::getAABB(const index_t& i) const {
		return { min + vec_t(i) * boxSize, min + vec_t(i + index_t(1)) * boxSize };
	}

	template <class T>
	Grid<T>::Iterator::Iterator(Grid<T>* g, const index_t& index)
		: grid(g), current(index) {}

	template <class T>
	Grid<T>::Iterator::Iterator(Grid<T>* g, const Ray& ray) : grid(g) {
		scalar_t t;
		if (!g || !g->rayHit(ray, t)) {
			g = nullptr;
			current = { MAX_INDEX, MAX_INDEX, MAX_INDEX };
			return;
		}
		p = ray.at(t);
		current = g->worldToGrid(p);
		D = ray.D;
		step = glm::sign(D);
		abstep = glm::abs(step);
	}

	template <class T> typename Grid<T>::Iterator& Grid<T>::Iterator::operator++() {
		if (!grid)
			return *this;
		const aabb_t box = grid->getAABB(current);
		const point_t pivot = vec_t(abstep) * box.max + vec_t(-abstep + 1) * box.min;

		const vec_t next = (pivot - p) / (D + EPSILON);
		const scalar_t min = glm::min(glm::min(next.x, next.y), next.z);
		for (size_t i = 0; i < DIM; i++) {
			if (glm::abs(next[i] - min) < EPSILON) {
				current[i] += step[i];
				p += min * grid->boxSize;
			}
		}
		if (glm::any(glm::lessThan(current, index_t(0))) ||
			glm::any(glm::greaterThanEqual(current, grid->dimensions))) {
			grid = nullptr;
			current = index_t(MAX_INDEX);
		}
		return *this;
	}

	template <class T>
	typename Grid<T>::bucket_t& Grid<T>::Iterator::operator*() const {
		return (*grid)[current];
	}

	template <class T>
	typename Grid<T>::bucket_t* Grid<T>::Iterator::operator->() const {
		return &(*grid)[current];
	}

	template <class T>
	bool Grid<T>::Iterator::operator==(const Iterator& other) const {
		return this->grid == other.grid && this->current == other.current;
	}
	template <class T>
	bool Grid<T>::Iterator::operator!=(const Iterator& other) const {
		return !(*this == other);
	}

	template <class T>
	const index_t Grid<T>::Iterator::getIndex() const {
		return current;
	}
}; // namespace war
