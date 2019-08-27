#include <meevax/library/posix.hpp>
#include <meevax/system/environment.hpp>

namespace meevax::posix
{
  extern "C" PROCEDURE(export_library)
  {
    using namespace meevax::system;

    environment library {};

    library.define<procedure>("dummy", dummy);

    return make<environment>(library);
  }
} // namespace meevax::posix

