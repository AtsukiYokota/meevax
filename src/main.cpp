#include <limits>

#include <boost/cstdlib.hpp>

#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include <meevax/posix/linker.hpp>
#include <meevax/system/environment.hpp>
#include <meevax/utility/demangle.hpp>

#include <meevax/protocol/connection.hpp>
#include <meevax/protocol/event.hpp>
#include <meevax/visual/context.hpp>
#include <meevax/visual/surface.hpp>

int main() try
{
  meevax::system::environment program {meevax::system::scheme_report_environment<7>};

  // for (program.open("/dev/stdin"); program.ready(); ) try
  // {
  //   std::cout << "\n> " << std::flush;
  //   const auto expression {program.read()};
  //   std::cerr << "\n; read    \t; " << expression << std::endl;
  //
  //   const auto executable {program.compile(expression)};
  //
  //   const auto evaluation {program.execute(executable)};
  //   std::cerr << "; => " << std::flush;
  //   std::cout << evaluation << std::endl;
  // }
  // catch (const object& something) // runtime exception generated by user code
  // {
  //   std::cerr << something << std::endl;
  //   continue;
  // }
  // catch (const exception& exception) // TODO REMOVE THIS
  // {
  //   std::cerr << exception << std::endl;
  //   continue; // TODO EXIT IF NOT IN INTARACTIVE MODE
  //   // return boost::exit_exception_failure;
  // }

  const meevax::protocol::connection connection {};

  meevax::visual::surface surface {connection};
  {
    surface.map();

    surface.configure(XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, 1280u, 720u);
    surface.size(1280, 720);

    surface.change_attributes(XCB_CW_EVENT_MASK,
        XCB_EVENT_MASK_NO_EVENT
      | XCB_EVENT_MASK_KEY_PRESS
      // | XCB_EVENT_MASK_KEY_RELEASE
      // | XCB_EVENT_MASK_BUTTON_PRESS
      // | XCB_EVENT_MASK_BUTTON_RELEASE
      // | XCB_EVENT_MASK_ENTER_WINDOW
      // | XCB_EVENT_MASK_LEAVE_WINDOW
      // | XCB_EVENT_MASK_POINTER_MOTION
      // | XCB_EVENT_MASK_POINTER_MOTION_HINT
      // | XCB_EVENT_MASK_BUTTON_1_MOTION
      // | XCB_EVENT_MASK_BUTTON_2_MOTION
      // | XCB_EVENT_MASK_BUTTON_3_MOTION
      // | XCB_EVENT_MASK_BUTTON_4_MOTION
      // | XCB_EVENT_MASK_BUTTON_5_MOTION
      // | XCB_EVENT_MASK_BUTTON_MOTION
      // | XCB_EVENT_MASK_KEYMAP_STATE
      | XCB_EVENT_MASK_EXPOSURE
      // | XCB_EVENT_MASK_VISIBILITY_CHANGE
      | XCB_EVENT_MASK_STRUCTURE_NOTIFY
      // | XCB_EVENT_MASK_RESIZE_REDIRECT
      // | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
      // | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
      // | XCB_EVENT_MASK_FOCUS_CHANGE
      // | XCB_EVENT_MASK_PROPERTY_CHANGE
      // | XCB_EVENT_MASK_COLOR_MAP_CHANGE
      // | XCB_EVENT_MASK_OWNER_GRAB_BUTTON
    );

    const std::string title {"Meevax Lisp System 0"};

    xcb_change_property(
      connection,
      XCB_PROP_MODE_REPLACE,
      surface.identity,
      XCB_ATOM_WM_NAME,
      XCB_ATOM_STRING,
      8,
      title.size(),
      title.c_str()
    );
  }
  xcb_flush(connection);

  for (meevax::protocol::event event {nullptr}; event.reset(xcb_wait_for_event(connection)), event; xcb_flush(connection))
  {
    std::cerr << "; event " << event.type() << "\t; " << event->sequence << " ; ";

    switch (event.type())
    {
    case XCB_KEY_PRESS:                                                    //  2
      std::cerr << "key-press" << std::endl;
      break;

    case XCB_KEY_RELEASE:                                                  //  3
    case XCB_BUTTON_PRESS:                                                 //  4
    case XCB_BUTTON_RELEASE:                                               //  5
    case XCB_MOTION_NOTIFY:                                                //  6
    case XCB_ENTER_NOTIFY:                                                 //  7
    case XCB_LEAVE_NOTIFY:                                                 //  8
    case XCB_FOCUS_IN:                                                     //  9
    case XCB_FOCUS_OUT:                                                    // 10
    case XCB_KEYMAP_NOTIFY:                                                // 11
      std::cerr << "unimplemented" << std::endl;
      break;

    case XCB_EXPOSE:                                                       // 12
      std::cerr << "expose" << std::endl;
      {
        meevax::visual::context context {surface};
        {
          context.set_source_rgb(0xF5 / 256.0, 0xF5 / 256.0, 0xF5 / 256.0);
          context.paint();
        }
      }
      break;

    case XCB_GRAPHICS_EXPOSURE:                                            // 13
    case XCB_NO_EXPOSURE:                                                  // 14
    case XCB_VISIBILITY_NOTIFY:                                            // 15
    case XCB_CREATE_NOTIFY:                                                // 16
    case XCB_DESTROY_NOTIFY:                                               // 17
    case XCB_UNMAP_NOTIFY:                                                 // 18
    case XCB_MAP_NOTIFY:                                                   // 19
    case XCB_MAP_REQUEST:                                                  // 20
    case XCB_REPARENT_NOTIFY:                                              // 21
      std::cerr << "unimplemented" << std::endl;
      break;

    case XCB_CONFIGURE_NOTIFY:                                             // 22
      std::cerr << "configure-notify" << std::endl;
      {
        const auto notify {reinterpret_cast<xcb_configure_notify_event_t*>(event.get())};
        surface.size(notify->width, notify->height);
      }
      break;

    case XCB_CONFIGURE_REQUEST:                                            // 23
      std::cerr << "configure-request" << std::endl;
      break;

    case XCB_GRAVITY_NOTIFY:                                               // 24
    case XCB_RESIZE_REQUEST:                                               // 25
    case XCB_CIRCULATE_NOTIFY:                                             // 26
    case XCB_CIRCULATE_REQUEST:                                            // 27
    case XCB_PROPERTY_NOTIFY:                                              // 28
    case XCB_SELECTION_CLEAR:                                              // 29
    case XCB_SELECTION_REQUEST:                                            // 30
    case XCB_SELECTION_NOTIFY:                                             // 31
    case XCB_COLORMAP_NOTIFY:                                              // 32
    case XCB_CLIENT_MESSAGE:                                               // 33
    case XCB_MAPPING_NOTIFY:                                               // 34
    case XCB_GE_GENERIC:                                                   // 35
      std::cerr << "unimplemented" << std::endl;
      break;
    }
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

