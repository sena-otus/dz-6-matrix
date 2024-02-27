#pragma once

#include <map>
#include <limits>
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

    /**
     * Тип для хранения ключа-индекса в мапе
     * */
  struct Idx
  {
    auto operator<=>(const Idx& other) const noexcept {
      if (auto cmp = i <=> other.i; cmp != 0)
        return cmp;
      return j <=> other.j;
    }

    bool operator==(const Idx& other) const = default;

    idx_t i; ///<! строка
    idx_t j; ///<! столбец
  };

    /** Тип мапы хранящей разреженную матрицу */
  using con_t = std::map<Idx, T>;


    /**
     * @brief доступ по индексам
     * @return прокси-объект, позволяющий осуществить вставку и удаление
     *   из разреженной матрицы, а также возврат значения по-умлочанию
     * */
  proxy_obj<T, defval> operator()(idx_t i, idx_t j)
  {
    return proxy_obj<T,defval>(*this, Idx{i, j});
  }

    /** количество заполненных элементов */
  std::size_t size() const { return m_array.size(); }

    /**
     * @brief установить значение.
     * Если значение равно значению по-умолчанию, элемент из по индексу
     * удалится. Если элемента по индексу нет, он добавляется.
     * @Idx индекс
     * @val значение
     * */
  void set(const Idx &idx, T val) noexcept
  {
    if(val == defval)
    {
      auto it = m_array.find(idx);
      if(it != m_array.end()) m_array.erase(it);
    }
    else
    {
      m_array[idx] = val;
    }
  }

    /**
     * @brief Получить значение.
     * Если элемент по индексу отсутствует, возвращатся значение по-умолчанию.
     * @Idx индекс
     * @return значение
     * */
  T get(const Idx &idx) const noexcept
  {
    auto vali = m_array.find(idx);
    if(vali == m_array.end()) {
      return defval;
    }
    return vali->second;
  }


    /** @brief Константаный итератор минимальная имплементация */
  class const_iterator
  {
    friend class Matrix<T, defval>;
    using mmap_const_iterator = typename con_t::const_iterator;
  public:
    const_iterator() noexcept
    : m_mmit() { }

    explicit
    const_iterator(mmap_const_iterator mmit) noexcept
      : m_mmit(mmit) { }

    const_proxy_obj<T, defval> operator*() const noexcept {
      return const_proxy_obj<T,defval>(m_mmit);
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
    mmap_const_iterator m_mmit;
  };

    /** @brief Итератор минимальная имплементация */
  class iterator
  {
    friend class Matrix<T, defval>;
    using mmap_iterator = typename con_t::iterator;
  public:
    iterator() noexcept
    : m_mmit() { }

    explicit
    iterator(Matrix<T, defval> *m, mmap_iterator mmit) noexcept
      : m_m{m}, m_mmit(mmit) { }

    proxy_obj<T, defval> operator*() const noexcept {
      return proxy_obj<T,defval>(*m_m, Idx{m_mmit->first.i, m_mmit->first.j});
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
      return const_iterator(m_mmit);
    }

  private:
    Matrix<T, defval> *m_m{nullptr};
    mmap_iterator m_mmit;
  };

  iterator begin() noexcept { return iterator(this, m_array.begin());}
  const_iterator cbegin() const noexcept { return const_iterator(m_array.begin()); }
  const_iterator begin() const noexcept { return cbegin(); }
  iterator end() noexcept { return iterator(this, m_array.end()); }
  const_iterator cend() const noexcept { return const_iterator(m_array.end()); }
  const_iterator end() const noexcept { cend(); }

private:
  con_t m_array; ///<! мапа для хранения разреженной матрицы
};


/**
 * @brief Прокси-объект для специальных операций при доступе к возвращаемому по индексу объекту.
 * Он добавляет элемент в массив, если его нет и удаляет элемент при присвоении значения по-умлочанию.
 * Также возвращает значение по-умолчанию при отсутствии элемента.
 * */
template<typename T, T defval>
class proxy_obj
{
  using m_t   = Matrix<T, defval>;
  using idx_t = typename m_t::idx_t;
  using val_t = typename m_t::val_t;
  using con_t = typename m_t::con_t;
  using Idx   = typename m_t::Idx;
public:
  proxy_obj() = default;

  explicit proxy_obj(m_t &m, const Idx &idx)
    : m_aref(m), m_idx{idx} {};

  proxy_obj& operator=(const T& val) noexcept
  {
    m_aref.set(m_idx, val);
    return *this;
  }

  bool operator==(const T &val) const noexcept
  {
    return m_aref.get(m_idx) == val;
  }

  explicit operator T() const noexcept { return m_aref.get(m_idx); }

    /** Для работы примера из задания нужны преобразования в тупл */
  template<typename T1, typename T2, typename T3>
  operator std::tuple<T1, T2, T3>() noexcept {
    m_v = m_aref.get(m_idx);
    return std::tuple<T1, T2, T3>(m_idx.i, m_idx.j, m_v);
  }

    /** Для работы примера из задания нужны преобразования в тупл */
  template<typename T1, typename T2, typename T3>
  operator std::tuple<T1&, T2&, T3&>() noexcept {
    m_v = m_aref.get(m_idx);
    return std::tuple<T1&, T2&, T3&>(m_idx.i, m_idx.j, m_v);
  }

    /** Для работы примера из задания нужны преобразования в тупл */
  template<typename T1, typename T2, typename T3>
  operator std::tuple<const T1&, const T2&, const T3&>() const noexcept {
    m_v = m_aref.get(m_idx);
    return std::tuple<const T1&, const T2&, const T3&>(m_idx.i, m_idx.j, m_v);
  }

  Idx idx() const { return m_idx;}
  val_t val() const { return m_aref.get(m_idx);}

private:
  m_t& m_aref;
  Idx m_idx;
  mutable val_t m_v;
};


/**
 * @brief Константный прокси-объект для доступа к возвращаемому по индексу объекту.
 * Нужен для обеспечения совместимости с обычным прокси-объектом.
 * */
template<typename T, T defval>
class const_proxy_obj
{
  using m_t = Matrix<T, defval>;
  using idx_t = typename m_t::idx_t;
  using val_t = typename m_t::val_t;
  using con_t = typename m_t::con_t;
public:
  const_proxy_obj() = default;

    /** Константный прокси объект может быть использован только для
     * доступа на чтение. Он никогда не добавляет значений в мартицу.
     * @param mmit итератор никогда не должен указывать на end()
     * */
  explicit
  const_proxy_obj(typename con_t::const_iterator mmit)
    : m_mmit(mmit) { }

  bool operator==(const T &val) const noexcept
  {
    return m_mmit->second == val;
  }

  explicit operator T() const noexcept { return m_mmit->second; }

  template<typename T1, typename T2, typename T3>
  operator std::tuple<T1, T2, T3>() noexcept {
    return std::tuple<T1, T2, T3>(m_mmit->first.i, m_mmit->first.j, m_mmit->second);
  }

  template<typename T1, typename T2, typename T3>
  operator std::tuple<const T1&, const T2&, const T3&>() noexcept {
    return std::tuple<const T1&, const T2&, const T3&>(m_mmit->first.i, m_mmit->first.j, m_mmit->second);
  }
private:
  typename con_t::const_iterator m_mmit;
};
