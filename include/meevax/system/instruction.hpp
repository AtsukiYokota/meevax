#ifndef INCLUDED_MEEVAX_SYSTEM_INSTRUCTION_HPP
#define INCLUDED_MEEVAX_SYSTEM_INSTRUCTION_HPP

#include <boost/preprocessor.hpp>

#include <meevax/system/pair.hpp>

namespace meevax::system
{
  #ifdef SEQ
  #undef SEQ
  #endif

  #define SEQ \
    (APPLY) \
    (APPLY_TAIL) \
    (DEFINE) \
    (JOIN) \
    (LOAD_GLOBAL) \
    (LOAD_LITERAL) \
    (LOAD_LOCAL) \
    (LOAD_LOCAL_VARIADIC) \
    (MAKE_CLOSURE) \
    (MAKE_CONTINUATION) \
    (MAKE_MODULE) \
    (POP) \
    (PUSH) \
    (RETURN) \
    (SELECT) \
    (SELECT_TAIL) \
    (SET_GLOBAL) \
    (SET_LOCAL) \
    (SET_LOCAL_VARIADIC) \
    (STOP)

  enum class code
  {
    BOOST_PP_SEQ_ENUM(SEQ)
  };

  struct instruction
  {
    const code value;

    template <typename... Ts>
    instruction(Ts&&... args)
      : value {std::forward<Ts>(args)...}
    {}
  };

  #define INSTRUCTION_CASE(unused, data, elem) \
  case code::elem: \
    os << BOOST_PP_STRINGIZE(elem); \
    break;

  std::ostream& operator<<(std::ostream& os, const instruction& instruction)
  {
    os << "\x1b[32m";

    switch (instruction.value)
    {
    BOOST_PP_SEQ_FOR_EACH(INSTRUCTION_CASE, ~, SEQ)
    }

    return os << "\x1b[0m";
  }

  #define DEFINE_INSTRUCTION_LITERAL(unused, data, elem) \
  static const auto _##elem##_

  static const auto _apply_               {make<instruction>(code::APPLY)};
  static const auto _apply_tail_          {make<instruction>(code::APPLY_TAIL)};
  static const auto _define_              {make<instruction>(code::DEFINE)};
  static const auto _join_                {make<instruction>(code::JOIN)};
  static const auto _load_global_         {make<instruction>(code::LOAD_GLOBAL)};
  static const auto _load_literal_        {make<instruction>(code::LOAD_LITERAL)};
  static const auto _load_local_          {make<instruction>(code::LOAD_LOCAL)};
  static const auto _load_local_variadic_ {make<instruction>(code::LOAD_LOCAL_VARIADIC)};
  static const auto _make_closure_        {make<instruction>(code::MAKE_CLOSURE)};
  static const auto _make_continuation_   {make<instruction>(code::MAKE_CONTINUATION)};
  static const auto _make_module_         {make<instruction>(code::MAKE_MODULE)};
  static const auto _pop_                 {make<instruction>(code::POP)};
  static const auto _push_                {make<instruction>(code::PUSH)};
  static const auto _return_              {make<instruction>(code::RETURN)};
  static const auto _select_              {make<instruction>(code::SELECT)};
  static const auto _select_tail_         {make<instruction>(code::SELECT_TAIL)};
  static const auto _set_global_          {make<instruction>(code::SET_GLOBAL)};
  static const auto _set_local_           {make<instruction>(code::SET_LOCAL)};
  static const auto _set_local_variadic_  {make<instruction>(code::SET_LOCAL_VARIADIC)};
  static const auto _stop_                {make<instruction>(code::STOP)};
} // namespace meevax::system

#endif // INCLUDED_MEEVAX_SYSTEM_INSTRUCTION_HPP

