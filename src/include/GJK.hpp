#pragma once
namespace war {
template <class shape1_t, class shape2_t>
bool GJKcheck(const shape1_t &s1, const shape2_t &s2);
}
#include <GJK.ipp>
