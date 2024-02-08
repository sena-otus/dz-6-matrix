#pragma once

#include <map>
#include <limits>
#include <boost/multiprecision/cpp_int.hpp>


template<typename T, T defval> class proxy_obj;

/**
 * @brief matrix class
 *  */
template <typename T, T defval>
class Matrix
{
  using mul_t = boost::multiprecision::uint128_t;
public:
  using idx_t = uint64_t;
  using con_t = std::map<boost::multiprecision::uint128_t, T>;
  proxy_obj<T, defval> operator()(idx_t i, idx_t j)
  {
    return proxy_obj<T,defval>(*this, i, j);
  }

  size_t size() const { return m_array.size(); }

  void set(idx_t i, idx_t j, T val) noexcept
  {
    m_array[key(i, j)] = val;
  }

  T get(idx_t i, idx_t j) const noexcept
  {
    auto vali = m_array.find(key(i, j));
    if(vali == m_array.end()) {
      return defval;
    }
    return vali->second;
  }

  typename con_t::iterator begin() noexcept
  {
    return m_array.begin();
  }

  typename con_t::const_iterator begin() const noexcept
  {
    return m_array.begin();
  }

  typename con_t::iterator end() noexcept
  {
    return m_array.end();
  }

  typename con_t::const_iterator end() const noexcept
  {
    return m_array.end();
  }
private:
  mul_t key(idx_t i, idx_t j) const noexcept
  {
    mul_t i2{i};
    mul_t j2{j};
    return std::numeric_limits<idx_t>::max() * i2 + j2;
  }

   con_t m_array;
};


template<typename T, T defval>
class proxy_obj
{
  using m_t = Matrix<T, defval>;
public:

  proxy_obj(Matrix<T, defval>& m, typename m_t::idx_t i, typename m_t::idx_t j)
    : m_aref(m), m_i(i), m_j(j) {}

  proxy_obj& operator=(const T& val) noexcept
  {
    m_aref.set(m_i, m_j, val);
    return *this;
  }

  bool operator==(const T &val) const noexcept
  {
    return m_aref.get(m_i, m_j) == val;
  }

  explicit operator T() const noexcept { return m_aref.get(m_i, m_j); }
private:
  m_t& m_aref;
  typename m_t::idx_t m_i;
  typename m_t::idx_t m_j;
};
