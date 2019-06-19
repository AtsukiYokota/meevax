#include <meevax/posix/linker.hpp>
#include <meevax/system/environment.hpp>
#include <meevax/utility/demangle.hpp>

#include <boost/cstdlib.hpp>

int main() try
{
  using namespace meevax::system;

  environment program {scheme_report_environment<7>};
  {
    // (dynamic-link-open <path>)
    program.define<procedure>("dynamic-link-open", [&](auto&& args)
    {
      if (auto size {length(args)}; size < 1)
      {
        throw error {"procedure dynamic-link-open expects a string for argument, but received nothing."};
      }
      else if (const object& s {car(args)}; not s.is<string>())
      {
        throw error {
                "procedure dynamic-link-open expects a string for argument, but received ",
                meevax::utility::demangle(s.type()),
                " rest ", size, " argument",
                (size < 2 ? " " : "s "),
                "were ignored."
              };
      }
      else
      {
        return make<meevax::posix::linker>(s.template as<string>());
      }
    });

    // (dynamic-link-procedure <linker> <name>)
    program.define<procedure>("dynamic-link-procedure", [&](auto&& args)
    {
      if (auto size {length(args)}; size < 1)
      {
        throw error {"procedure dynamic-link-procedure expects two arguments (linker and string), but received nothing."};
      }
      else if (size < 2)
      {
        throw error {"procedure dynamic-link-procedure expects two arguments (linker and string), but received only one argument."};
      }
      else if (const auto& linker {car(args)}; not linker.template is<meevax::posix::linker>())
      {
        throw error {
                "procedure dynamic-link-open expects a linker for first argument, but received ",
                meevax::utility::demangle(linker.type()),
                " rest ", size - 1, " argument",
                (size < 2 ? " " : "s "),
                "were ignored."
              };
      }
      else if (const auto& name {cadr(args)}; not name.template is<string>())
      {
        throw error {
                "procedure dynamic-link-open expects a string for second argument, but received ",
                meevax::utility::demangle(name.type()),
                " rest ", size - 2, " argument",
                (size < 3 ? " " : "s "),
                "were ignored."
              };
      }
      else
      {
        const auto& linker_ {car(args).template as<meevax::posix::linker>()};
        const std::string& name_ {cadr(args).template as<string>()};
        return make<procedure>(
                 name_, // TODO リンクしてるライブラリ名を名前に含めること（#<procedure hoge from libhoge.so>）
                 linker_.template link<typename procedure::signature>(name_)
               );
      }
    });
  }

  for (program.open("/dev/stdin"); program.ready(); ) try
  {
    std::cout << "\n> " << std::flush;
    const auto expression {program.read()};
    std::cerr << "\n; read    \t; " << expression << std::endl;

    const auto executable {program.compile(expression)};

    const auto evaluation {program.execute(executable)};
    std::cerr << "; => " << std::flush;
    std::cout << evaluation << std::endl;
  }
  catch (const object& something) // runtime exception generated by user code
  {
    std::cerr << something << std::endl;
    continue;
  }
  catch (const exception& exception) // TODO REMOVE THIS
  {
    std::cerr << exception << std::endl;
    continue; // TODO EXIT IF NOT IN INTARACTIVE MODE
    // return boost::exit_exception_failure;
  }

  return boost::exit_success;
}
catch (const std::exception& error)
{
  std::cout << "\x1b[1;31m" << "unexpected standard exception: \"" << error.what() << "\"" << "\x1b[0m" << std::endl;
  return boost::exit_exception_failure;
}
catch (...)
{
  std::cout << "\x1b[1;31m" << "unexpected exception occurred." << "\x1b[0m" << std::endl;
  return boost::exit_exception_failure;
}

