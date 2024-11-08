#include <common.hpp>
namespace war {
	scalar_t min(const vec_t& vec) {
		scalar_t m = vec[0];
		m = glm::min(m, vec[1]);
		m = glm::min(m, vec[2]);
		return m;
	}
	scalar_t max(const vec_t& vec) {
		scalar_t m = vec[0];
		m = glm::max(m, vec[1]);
		m = glm::max(m, vec[2]);
		return m;
	}

} // namespace war
