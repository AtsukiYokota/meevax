#ifndef INCLUDED_MEEVAX_CORE_ACCESSOR_HPP
#define INCLUDED_MEEVAX_CORE_ACCESSOR_HPP

#include <memory>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace meevax::core
{
  template <typename T>
  struct accessor
    : public std::shared_ptr<T>,
      public std::iterator<std::input_iterator_tag, typename std::shared_ptr<T>::element_type>
  {
    template <typename U>
    struct binder
      : public U,
        public virtual T
    {
      template <typename... Ts>
      explicit constexpr binder(Ts&&... args)
        : std::conditional<std::is_base_of<T, U>::value, T, U>::type {std::forward<Ts>(args)...}
      {}

      auto type() const noexcept
        -> const std::type_info& final override
      {
        return typeid(U);
      }
    };

    #define meevax_core_indirect_accessor(name, index) \
    friend decltype(auto) name(const accessor<T>& accessor) noexcept \
    { \
      return std::get<index>(accessor.std::shared_ptr<T>::operator*()); \
    }

  public:
    template <typename... Ts>
    constexpr accessor(Ts&&... args)
      : std::shared_ptr<T> {std::forward<Ts>(args)...}
    {}

    meevax_core_indirect_accessor(car, 0);
    meevax_core_indirect_accessor(cdr, 1);

    template <typename U, typename... Ts>
    static constexpr decltype(auto) bind(Ts&&... args)
    {
      return std::make_shared<binder<U>>(std::forward<Ts>(args)...);
    }

  public: // iterator supports
    decltype(auto) operator*() const noexcept
    {
      return car(*this);
    }

    decltype(auto) operator++() noexcept
    {
      return *this = cdr(*this);
    }

  public: // stack supports
    using size_type = std::size_t;

    using reference = accessor<T>&;
    using const_reference = const reference;

    decltype(auto) back() noexcept
    {
      return *this ? operator*() : *this;
    }

    decltype(auto) push_back(const accessor<T>& access)
    {
      return *this = std::make_shared<T>(access, *this);
    }

    decltype(auto) pop_back()
    {
      return operator++();
    }

    template <typename... Ts>
    decltype(auto) emplace_back(Ts&&... args)
    {
      return *this = std::make_shared<T>(std::forward<Ts>(args)..., *this);
    }
  };
} // namespace meevax::core

#endif // INCLUDED_MEEVAX_CORE_ACCESSOR_HPP
