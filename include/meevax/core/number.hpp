#ifndef INCLUDED_MEEVAX_CORE_NUMBER_HPP
#define INCLUDED_MEEVAX_CORE_NUMBER_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <boost/multiprecision/gmp.hpp>

#include <meevax/core/pair.hpp>

namespace meevax::core
{
  using number = boost::multiprecision::mpf_float;

  std::ostream& operator<<(std::ostream& os, const number& number)
  {
    return os << "\x1B[36m" << number.str() << "\x1B[0m";
  }

  cursor operator+(const cursor& lhs, const cursor& rhs)
  {
    return cursor::bind<number>(
      lhs.data().as<number>() + rhs.data().as<number>()
    );
  }
} // namespace meevax::core

#endif // INCLUDED_MEEVAX_CORE_NUMBER_HPP

