#ifndef INCLUDED_MEEVAX_SYSTEM_PROCEDURE_HPP
#define INCLUDED_MEEVAX_SYSTEM_PROCEDURE_HPP

#include <functional> // std::funstion

#include <meevax/system/cursor.hpp>

namespace meevax::system
{
  struct procedure
    : public std::function<cursor (const cursor&)>
  {
    using signature = cursor (*)(const cursor&);

    const std::string name;

    template <typename... Ts>
    procedure(const std::string& name, Ts&&... args)
      : std::function<cursor (const cursor&)> {std::forward<Ts>(args)...}
      , name {name}
    {}
  };

  std::ostream& operator<<(std::ostream& os, const procedure& procedure)
  {
    return os << "#<procedure " << procedure.name << ">";
  }
} // namespace meevax::system

#endif // INCLUDED_MEEVAX_SYSTEM_PROCEDURE_HPP

