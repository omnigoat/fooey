#include <fooey/widgets/window.hpp>

#include <fooey/events/move.hpp>
#include <fooey/events/resize.hpp>

#include <string>
#include <tuple>

using namespace fooey;
using fooey::window_t;
using fooey::window_ptr;


window_t::window_t(properties::captioned_t::caption_t const& c, uint32_t width, uint32_t height)
	: widget_t(width, height), captioned_t(c), fullscreen_()
{
	on({
		{"move.internal", [&](events::move_t& e) {
			left_ = e.left();
			top_ = e.top();
		}},

		{"resize.internal", [&](events::resize_t& e) {
			if (!fullscreen_) {
				width_ = e.width();
				height_ = e.height();
			}
		}}
	});
}

window_t::~window_t()
{
}
