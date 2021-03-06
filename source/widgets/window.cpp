#include <fooey/widgets/window.hpp>

#include <fooey/events/move.hpp>
#include <fooey/events/resize.hpp>

#include <string>
#include <tuple>

using namespace fooey;
using fooey::window_t;
using fooey::window_ptr;


window_t::window_t(properties::captioned_t::caption_t const& c, uint32 width, uint32 height)
	: widget_t(width, height), captioned_t(c), fullscreen_(), window_state_(window_state_t::windowed),
	  dc_width_(), dc_height_()
{
	on({
		{"move.internal", [&](events::move_t const& e) {
			left_ = e.left();
			top_ = e.top();
		}},

		{"resize.internal", [&](events::resize_t const& e) {
			width_ = e.width();
			height_ = e.height();
		}},

		{"resize-dc.internal", [&](events::resize_t const& e) {
			dc_width_ = e.width();
			dc_height_ = e.height();
		}},

		{"minimized.internal", [&]() {
			window_state_ = window_state_t::minimized;
		}},

		{"maximized.internal", [&]() {
			window_state_ = window_state_t::maximized;
		}},

		{"restored.internal", [&]() {
			window_state_ = window_state_t::windowed;
		}}

	});
}

window_t::~window_t()
{
}
