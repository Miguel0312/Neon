#ifndef NEON_INTERVAL_H
#define NEON_INTERVAL_H

#include <algorithm>
#include <limits>
namespace Neon {
template <typename T> class Interval {
public:
  // Creates an empty interval
  Interval()
      : m_min(std::numeric_limits<T>::max()),
        m_max(std::numeric_limits<T>::min()) {}

  Interval(const T &min, const T &max) : m_min(min), m_max(max) {}

  bool contains(T val, bool exclusive = false) const {
    if (exclusive)
      return m_min < val && m_max > val;
    return m_min <= val && m_max >= val;
  }

  // Expand the min and max values by delta
  Interval<T> expanded(const T &delta) const {
    return Interval<T>(m_min - delta, m_max + delta);
  }

  // Expands in place
  Interval<T> &expand(const T &delta) {
    m_min -= delta, m_max += delta;
    return *this;
  }

  // Returns the union of two intervals
  Interval<T> expanded(const Interval<T> &interval) const {
    return Interval<T>(std::min(m_min, interval.m_min),
                       std::max(m_min, interval.m_min));
  }

  // Expands in place
  Interval<T> &expand(const Interval<T> &interval) {
    m_min = std::min(m_min, interval.m_min);
    m_max = std::max(m_max, interval.m_max);
    return *this;
  }

  // Returns the intersection of two intervals
  Interval<T> intersection(const Interval<T> &interval) const {
    return Interval<T>(std::max(m_min, interval.m_min),
                       std::min(m_min, interval.m_min));
  }

  // Returns wether or not the two intervals have a common point
  bool intersects(const Interval<T> &interval) const {
    return !(m_max <= interval.m_min || m_min >= interval.m_max);
  }

  // Returns true if the interval is not empty (m_min <= m_max)
  bool empty() const { return m_min <= m_max; }

private:
  T m_min, m_max;
};

typedef Interval<int> Intervali;
typedef Interval<float> Intervalf;
typedef Interval<double> Intervald;
} // namespace Neon

#endif
