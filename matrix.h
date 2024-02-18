#pragma once

#include <map>
#include <limits>
//#include <boost/multiprecision/cpp_int.hpp>
#include <tuple>


template<typename T, T defval> class proxy_obj;
template<typename T, T defval> class const_proxy_obj;

/**
 * @brief matrix class
 *  */
template <typename T, T defval>
class Matrix
{
public:
    /** Тип значения, хранящегося в массиве */
  using val_t = T;
    /** Тип используемый как индекс массива */
  using idx_t = int;
    /** Тип используемый как ключ в мапе, он хранит произведение idx_t * idx_t
     *  Чтобы использовать полный диапазон idx_t, sizeof(mul_t) должен
     *  быть равен 2*sizeof(idx_t). Можно использовать, например
     *  boost::multiprecision::uint128_t
     *  */
  using mul_t = long int;
  using con_t = std::map<mul_t, T>;

  proxy_obj<T, defval> operator()(idx_t i, idx_t j)
  {
    return proxy_obj<T,defval>(*this, i, j);
  }

  std::size_t size() const { return m_array.size(); }

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


  class const_iterator
  {
    friend class Matrix<T, defval>;
    using mmap_const_iterator = typename con_t::const_iterator;
  public:
    const_iterator() noexcept
    : m_mmit() { }

    explicit
    const_iterator(const Matrix<T, defval> *m, mmap_const_iterator mmit) noexcept
      : m_m(m), m_mmit(mmit) { }

    const_proxy_obj<T, defval> operator*() const noexcept {
      auto [i, j] = keys(m_mmit->first);
      return const_proxy_obj<T,defval>(*m_m, i, j);
    }
      // const_proxy_obj<T, defval>* operator->() const noexcept{ return &m_it->m_value; }

    const_iterator& operator++() noexcept {
      ++m_mmit;
      return *this;
    }

    const_iterator& operator--() noexcept {
      --m_mmit;
      return *this;
    }

    friend bool
    operator==(const const_iterator& x, const const_iterator& y) noexcept
      { return x.m_mmit == y.m_mmit; }

    friend bool
    operator!=(const const_iterator& x, const const_iterator& y) noexcept
      { return x.m_mmit != y.m_mmit; }

  private:
    const Matrix<T, defval> *m_m{nullptr};
    mmap_const_iterator m_mmit;
  };


  class iterator
  {
    friend class Matrix<T, defval>;
    using mmap_iterator = typename con_t::iterator;
  public:
    iterator() noexcept
    : m_mmit() { }

    explicit
    iterator(Matrix<T, defval> *m, mmap_iterator mmit) noexcept
      : m_m(m), m_mmit(mmit) { }

    proxy_obj<T, defval> operator*() const noexcept {
      auto [i, j] = keys(m_mmit->first);
      return proxy_obj<T,defval>(*m_m, i, j);
    }
      // proxy_obj<T, defval>* operator->() const noexcept{ return &m_it->m_value; }

    iterator& operator++() noexcept {
      ++m_mmit;
      return *this;
    }

    iterator& operator--() noexcept {
      --m_mmit;
      return *this;
    }

    friend bool
    operator==(const iterator& x, const iterator& y) noexcept
    { return x.m_mmit == y.m_mmit; }

    friend bool
    operator!=(const iterator& x, const iterator& y) noexcept
    { return x.m_mmit != y.m_mmit; }

    operator const_iterator() const noexcept
    {
      return const_iterator(m_m, m_mmit);
    }

  private:
    Matrix<T, defval> *m_m{nullptr};
    mmap_iterator m_mmit;
  };

  iterator begin() noexcept { return iterator(this, m_array.begin());}
  const_iterator cbegin() const noexcept { return const_iterator(this, m_array.begin()); }
  const_iterator begin() const noexcept { return cbegin(); }
  iterator end() noexcept { return iterator(this, m_array.end()); }
  const_iterator cend() const noexcept { return const_iterator(this, m_array.end()); }
  const_iterator end() const noexcept { cend(); }

  static std::pair<idx_t,idx_t> keys(const mul_t &k) noexcept
  {
    idx_t i = k / std::numeric_limits<idx_t>::max();
    idx_t j = k % std::numeric_limits<idx_t>::max();
    return {i,  j};
  }

private:
  static mul_t key(idx_t i, idx_t j) noexcept
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
  using idx_t = typename m_t::idx_t;
  using val_t = typename m_t::val_t;
public:

  proxy_obj(Matrix<T, defval>& m, idx_t i, idx_t j)
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

  template<typename T1, typename T2, typename T3>
  operator std::tuple<T1, T2, T3>() noexcept {
    m_v = m_aref.get(m_i, m_j);
    return std::tuple<T1, T2, T3>(m_i, m_j, m_v);
  }
private:
  m_t& m_aref;
  idx_t m_i;
  idx_t m_j;
  val_t m_v;
};


template<typename T, T defval>
class const_proxy_obj
{
  using m_t = Matrix<T, defval>;
  using idx_t = typename m_t::idx_t;
  using val_t = typename m_t::val_t;
public:

  const_proxy_obj(const Matrix<T, defval>& m, idx_t i, idx_t j)
    : m_aref(m), m_i(i), m_j(j) {}

  bool operator==(const T &val) const noexcept
  {
    return m_aref.get(m_i, m_j) == val;
  }

  explicit operator T() const noexcept { return m_aref.get(m_i, m_j); }

  template<typename T1, typename T2, typename T3>
  operator std::tuple<T1, T2, T3>() noexcept {
    m_v = m_aref.get(m_i, m_j);
    return std::tuple<T1, T2, T3>(m_i, m_j, m_v);
  }

private:
  const m_t& m_aref;
  idx_t m_i;
  idx_t m_j;
  val_t m_v;
};
