#include <fooey/widgets/window.hpp>

using namespace fooey;
using fooey::window_t;
using fooey::window_ptr;

window_t::window_t(properties::captioned_t::caption_t const& c)
	: captioned_t(c)
{
}

auto fooey::map_basic_window_events(window_ptr const& wnd) -> void
{
}
