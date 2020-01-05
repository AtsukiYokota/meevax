// #define THE_ONLY_SUBSET_OF_THE_EMPTY_SET_IS_ITSELF true

#include <meevax/kernel/syntactic_continuation.hpp>

int main(const int argc, char const* const* const argv) try
{
  meevax::kernel::syntactic_continuation program {meevax::kernel::layer<2>};

  /*============================================================================
  *
  * The environment system includes a command line option parser. The parser is
  * internally called the "configurator" and is primarily responsible for
  * changing the behavior of the environment.
  *
  *========================================================================== */
  program.configure(argc, argv);

  std::cerr << "; ice\t\t; All systems are go.\n";
  std::cerr << ";\t\t; You have control of root syntactic-continuation.\n";

  const auto prompt {"\n> "};

  for (program.open("/dev/stdin"); program.ready(); ) try
  {
    std::cout << prompt << std::flush;
    const auto expression {program.read()};
    std::cout << "\n";

    if (program.verbose.equivalent_to(meevax::kernel::true_object))
    {
      std::cerr << "; read\t\t; " << expression << std::endl;
    }

    const auto evaluation {program.evaluate(expression)};
    std::cout << evaluation << std::endl;
  }
  catch (const meevax::kernel::object& something) // runtime exception generated by user code
  {
    std::cerr << something << std::endl;
    continue;
  }
  catch (const meevax::kernel::exception& exception) // TODO REMOVE THIS
  {
    std::cerr << exception << std::endl;
    continue; // TODO EXIT IF NOT IN INTERACTIVE MODE
    // return boost::exit_exception_failure;
  }

  // auto value {meevax::kernel::make<float>(3.14)};
  //
  // auto x {value.as<double>()};
  // std::cout << "; pointer\t; " << x << std::endl;;
  //
  // auto y {value.as<int>()};
  // std::cout << "; pointer\t; " << y << std::endl;;

  return boost::exit_success;
}
catch (const meevax::kernel::exception& error)
{
  std::cerr << error << std::endl;
  return boost::exit_exception_failure;
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

