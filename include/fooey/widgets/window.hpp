#ifndef FOOEY_WIDGETS_WINDOW_HPP
#define FOOEY_WIDGETS_WINDOW_HPP
//======================================================================
#include <chrono>
#include <fooey/widget.hpp>
#include <fooey/properties.hpp>
#include <fooey/keys.hpp>
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
		window_t(properties::captioned_t::caption_t const& caption, uint32_t width, uint32_t height);
		~window_t();

		auto set_visible(bool) -> void;
		
		// events atma
		atma::event_t<widget_event_t> on_minimise;
		atma::event_t<widget_event_t> on_maximise;
		atma::event_t<widget_event_t> on_restore;
		atma::event_t<widget_event_t> on_close;
		atma::event_t<widget_event_t, uint32_t, uint32_t> on_resize;

		//
		key_state_t key_state;

		
		//auto drawable_region() const -> atma::math::rect_t;

		HWND hwnd;

	private:
		//auto on_changed_property(properties::event_t e) -> void override {}
	};

	typedef widget_ptr_t<window_t> window_ptr;


	inline auto window(std::string const& caption, uint32_t width, uint32_t height) -> window_ptr
	{
		return window_ptr(new window_t(caption, width, height));
	}


	auto map_basic_window_events(window_ptr const&) -> void;

//======================================================================
} // fooey
//======================================================================
#endif
//======================================================================
