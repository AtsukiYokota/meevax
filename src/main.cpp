#include <iostream>
#include <stdexcept>

#include <meevax/system/machine.hpp>
#include <meevax/system/modular.hpp>
#include <meevax/system/reader.hpp>

#include <boost/cstdlib.hpp>

int main()
{
  using namespace meevax::system;

  modular module {"main"};

  reader read {"/dev/stdin"};

  // XXX TEMPORARY
  machine machine {};
  {
    machine.define(module.intern("quote"), make<syntax>("quote", [&](auto&& exp, auto&&, auto&& continuation)
    {
      return cons(LDC, cadr(exp), continuation);
    }));

    machine.define(module.intern("car"), make<syntax>("car", [&](auto&& exp, auto&& scope, auto&& continuation)
    {
      return machine.compile(
               cadr(exp),
               scope,
               cons(CAR, continuation)
             );
    }));

    machine.define(module.intern("cdr"), make<syntax>("cdr", [&](auto&& exp, auto&& scope, auto&& continuation)
    {
      return machine.compile(
               cadr(exp),
               scope,
               cons(CDR, continuation)
             );
    }));

    machine.define(module.intern("cons"), make<syntax>("cons", [&](auto&& exp, auto&& scope, auto&& continuation)
    {
      return machine.compile(
               caddr(exp),
               scope,
               machine.compile(cadr(exp), scope, cons(CONS, continuation))
             );
    }));

    machine.define(module.intern("if"), make<syntax>("if", [&](auto&& exp, auto&& scope, auto&& continuation)
    {
      return machine.compile(
               cadr(exp), // conditional expression
               scope,
               cons(
                 SELECT,
                 machine.compile( caddr(exp), scope, list(JOIN)), // then expression
                 machine.compile(cadddr(exp), scope, list(JOIN)), // else expression
                 continuation
               )
             );
    }));

    machine.define(module.intern("define"), make<syntax>("define", [&](auto&& exp, auto&& scope, auto&& continuation)
    {
      return machine.compile(
               caddr(exp),
               scope,
               cons(DEFINE, cadr(exp), continuation)
             );
    }));

    machine.define(module.intern("lambda"), make<syntax>("lambda", [&](auto&& exp, auto&& scope, auto&& continuation)
    {
      return cons(
               LDF,
               machine.begin(
                 cddr(exp),
                 cons(
                   cadr(exp), // parameters
                   scope
                 ),
                 list(RETURN)
               ),
               continuation
             );
    }));

    machine.define(module.intern("pair?"), make<procedure>("pair?", [&](const cursor& args)
    {
      for (const cursor& each : args)
      {
        if (not each or not each.is<pair>())
        {
          return false_v;
        }
      }

      return true_v;
    }));

    machine.define(module.intern("eq?"), make<procedure>("eq?", [&](const cursor& args)
    {
      return car(args) == cadr(args) ? true_v : false_v;
    }));

    machine.define(module.intern("+"), make<procedure>("+", [&](const cursor& args)
    {
      return std::accumulate(args, unit, make<number>(0), std::plus {});
    }));

    machine.define(module.intern("*"), make<procedure>("*", [&](const cursor& args)
    {
      return std::accumulate(args, unit, make<number>(1), std::multiplies {});
    }));

    machine.define(module.intern("-"), make<procedure>("-", [&](const cursor& args)
    {
      // TODO LENGTH
      if (std::distance(args, unit) < 2)
      {
        return std::accumulate(args, unit, make<number>(0), std::minus {});
      }
      else
      {
        return std::accumulate(cursor {cdr(args)}, unit, car(args), std::minus {});
      }
    }));

    machine.define(module.intern("/"), make<procedure>("/", [&](const cursor& args)
    {
      return std::accumulate(args, unit, make<number>(1), std::divides {});
    }));
  }

  while (read) try
  {
    auto expression {read(module)};
    std::cerr << "[read] " << expression << std::endl;

    auto code {machine.compile(expression)};
    std::cerr << machine.execute(code) << "\n\n";
  }
  catch (const std::runtime_error& error)
  {
    std::cerr << "\x1B[31m[error] " << error.what() << "\x1B[0m\n\n";
    continue;
  }

  return boost::exit_success;
}

