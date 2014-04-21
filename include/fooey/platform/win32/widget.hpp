#pragma once
//======================================================================
#include <fooey/fooey_fwd.hpp>
#include <fooey/event.hpp>
#include <fooey/event_handler.hpp>

#include <atma/enable_multiple_shared_from_this.hpp>

#include <vector>
//======================================================================
namespace fooey
{
	struct widget_t : virtual atma::enable_multiple_shared_from_this, event_handler_t
	{
		typedef std::vector<widget_ptr> children_t;
		
		widget_t();
		widget_t(int32_t width, int32_t height);
		virtual ~widget_t();
		
		auto hwnd() const -> HWND { return hwnd_; }
		auto children() const -> children_t const& { return children_; }

		auto left() const -> int32_t { return left_; }
		auto top() const -> int32_t { return top_; }
		auto width() const -> int32_t { return width_; }
		auto height() const -> int32_t { return height_; }
		auto width_in_pixels() const -> int32_t { return width_; }
		auto height_in_pixels() const -> int32_t { return height_; }


		auto set_hwnd(HWND h) -> void { hwnd_ = h; }
		auto add_child(widget_ptr const&) -> void;

	protected:
		widget_wptr parent_;
		children_t children_;
		int32_t left_, top_, width_, height_;

	private:
		HWND hwnd_;
	};
}
