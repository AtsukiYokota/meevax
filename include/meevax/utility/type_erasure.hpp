#ifndef INCLUDED_MEEVAX_UTILITY_TYPE_ERASURE_HPP
#define INCLUDED_MEEVAX_UTILITY_TYPE_ERASURE_HPP

#include <typeinfo>
#include <utility>

#include <meevax/facade/identity.hpp>

namespace meevax::utility
{
  template <typename T, typename U>
  struct binder
    : public T,
      public U
  {
    template <typename... Ts>
    explicit constexpr binder(Ts&&... xs)
      : T {std::forward<Ts>(xs)...}
    {}

    auto type() const noexcept
      -> const std::type_info& override
    {
      return typeid(T);
    }
  };
} // namespace meevex::utility

#endif // INCLUDED_MEEVAX_UTILITY_TYPE_ERASURE_HPP

