#ifndef FOOEY_WIDGETS_WINDOW_HPP
#define FOOEY_WIDGETS_WINDOW_HPP
//======================================================================
#include <chrono>
#include <fooey/widget.hpp>
#include <fooey/properties.hpp>
#include <atma/event.hpp>
#include <atma/lockfree/queue.hpp>
#include <atma/config/platform.hpp>
//======================================================================
namespace fooey {
//======================================================================
	
	struct window_t :
		widget_t,
		properties::captioned_t
	{
		window_t(properties::captioned_t::caption_t const& caption);
		
		auto set_visible(bool) -> void;

		// events atma
		atma::event_t<> on_minimise;
		atma::event_t<> on_maximise;
		atma::event_t<> on_restore;
		atma::event_t<> on_close;
		atma::event_t<uint32_t, uint32_t> on_resize;

		HWND hwnd;

	private:
		auto on_changed_property(properties::event_t e) -> void override {}
	};

	typedef widget_ptr_t<window_t> window_ptr;


	inline auto window(std::string const& caption) -> window_ptr
	{
		return window_ptr(new window_t(caption));
	}


	auto map_basic_window_events(window_ptr const&) -> void;

//======================================================================
} // fooey
//======================================================================
#endif
//======================================================================
