#ifndef INCLUDED_MEEVAX_UTILITY_OVERLOAD_HPP
#define INCLUDED_MEEVAX_UTILITY_OVERLOAD_HPP

#include <type_traits>

namespace meevax::utility
{
  template <typename... Ts>
  struct overloaded
    : public Ts...
  {
    using Ts::operator()...;
  };

  template <typename... Ts>
  overloaded(Ts&&...)
    -> overloaded<Ts...>;

  template <typename... Ts>
  constexpr auto overload(Ts&&... operands)
    -> overloaded<typename std::decay<Ts...>::type>
  {
    return {std::forward<decltype(operands)>(operands)...};
  }
} // namespace meevax::utility

#endif // INCLUDED_MEEVAX_UTILITY_OVERLOAD_HPP

