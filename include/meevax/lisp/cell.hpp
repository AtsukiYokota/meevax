#ifndef INCLUDED_MEEVAX_LISP_CELL_HPP
#define INCLUDED_MEEVAX_LISP_CELL_HPP

#include <memory>
#include <string>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <utility>

#include <meevax/facade/identity.hpp>
#include <meevax/tuple/accessor.hpp>
#include <meevax/tuple/iterator.hpp>
#include <meevax/utility/heterogeneous_dictionary.hpp>
#include <meevax/utility/type_erasure.hpp>

namespace meevax::lisp
{
  class cell;

  using cursor = tuple::iterator<cell>;

  template <typename T>
  struct bind
  {
    template <typename... Ts>
    cursor operator()(Ts&&... args)
    {
      using binder = utility::binder<T, cell>;
      return std::make_shared<binder>(std::forward<Ts>(args)...);
    }
  };

  utility::heterogeneous_dictionary<cursor, bind<std::string>> symbols {
    std::make_pair("nil", cursor {nullptr})
  };

  struct cell
    : public std::tuple<cursor, cursor>,
      public facade::identity<cell>
  {
    template <typename T>
    constexpr cell(T&& head)
      : std::tuple<cursor, cursor> {std::forward<T>(head), symbols.intern("nil")}
    {}

    template <typename... Ts>
    constexpr cell(Ts&&... args)
      : std::tuple<cursor, cursor> {std::forward<Ts>(args)...}
    {}

    virtual ~cell() = default;
  };
} // namespace meevax::lisp

#endif // INCLUDED_MEEVAX_LISP_CELL_HPP

