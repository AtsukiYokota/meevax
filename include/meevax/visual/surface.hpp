#ifndef INCLUDED_MEEVAX_VISUAL_SURFACE_HPP
#define INCLUDED_MEEVAX_VISUAL_SURFACE_HPP

#include <cstdint>
#include <memory>

#include <cairo/cairo-xcb.h>

#include <meevax/protocol/accessor.hpp>
#include <meevax/protocol/machine.hpp>
#include <meevax/visual/context.hpp>

namespace meevax::visual
{
  constexpr auto events {
      XCB_EVENT_MASK_NO_EVENT
    // | XCB_EVENT_MASK_KEY_PRESS
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
  };

  class surface
    : public protocol::machine<surface, events>
    , public std::shared_ptr<cairo_surface_t>
  {
    using machine = protocol::machine<surface, events>;

  public:
    explicit surface(const protocol::connection& connection)
      : machine {connection, protocol::root_screen(connection)}
      , std::shared_ptr<cairo_surface_t> {
          cairo_xcb_surface_create(connection, identity, protocol::root_visualtype(connection), 1, 1),
          cairo_surface_destroy
        }
    {}

    explicit surface(const surface& surface)
      : machine {surface.connection, surface.identity}
      , std::shared_ptr<cairo_surface_t> {
          cairo_xcb_surface_create(connection, identity, protocol::root_visualtype(connection), 1, 1),
          cairo_surface_destroy
        }
    {}

    operator element_type*() const noexcept
    {
      return get();
    }

    decltype(auto) flush()
    {
      return cairo_surface_flush(*this);
    }

    void size(std::uint32_t width, std::uint32_t height) noexcept
    {
      cairo_xcb_surface_set_size(*this, width, height);
      flush();
    }

  public:
    void operator()(const std::unique_ptr<xcb_expose_event_t>)
    {
      context context {*this};
      context.set_source_rgb(0xF5 / 256.0, 0xF5 / 256.0, 0xF5 / 256.0);
      context.paint();
    }

    void operator()(const std::unique_ptr<xcb_configure_notify_event_t> event)
    {
      size(event->width, event->height);
    }
  };
} // namespace meevax::visual

#endif // INCLUDED_MEEVAX_VISUAL_SURFACE_HPP

