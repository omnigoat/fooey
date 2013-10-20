#ifndef FOOEY_PLATFORM_WIN32_WIDGET_HPP
#define FOOEY_PLATFORM_WIN32_WIDGET_HPP
//======================================================================
#include <memory>
#include <string>
#include <set>
#include <vector>
#include <initializer_list>
#include <chrono>
#include <atma/lockfree/queue.hpp>
#include <fooey/event.hpp>
#include <atma/config/platform.hpp>
//======================================================================
namespace fooey {
//======================================================================
	
	struct widget_t : std::enable_shared_from_this<widget_t>, event_handler_t
	{
		typedef std::vector<widget_ptr> children_t;
		
		widget_t();
		widget_t(uint32_t width, uint32_t height);
		virtual ~widget_t();
		
		auto hwnd() const -> HWND { return hwnd_; }
		auto width_in_pixels() const -> uint32_t { return width_; }
		auto height_in_pixels() const -> uint32_t { return height_; }
		auto children() const -> children_t const& { return children_; }

		auto set_hwnd(HWND h) -> void { hwnd_ = h; }
		auto add_child(widget_ptr const&) -> void;

	protected:
		widget_wptr parent_;
		children_t children_;
		uint32_t left_, top_, width_, height_;

	private:
		HWND hwnd_;
	};
		

//======================================================================
} // fooey
//======================================================================
#endif
//======================================================================
