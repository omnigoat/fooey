#ifndef FOOEY_WIDGETS_WINDOW_HPP
#define FOOEY_WIDGETS_WINDOW_HPP
//======================================================================
#include <fooey/widget.hpp>
#include <fooey/properties.hpp>
#include <atma/evented/event.hpp>
//======================================================================
namespace fooey {
//======================================================================
	
	struct window_t :
		widget_t,
		properties::captioned_t
	{
		window_t(properties::captioned_t::caption_t const& caption);
		
		auto set_visible(bool) -> void;


		// events
		atma::evented::event_t<int()> on_minimise;
		atma::evented::event_t<void()> on_maximise;
		atma::evented::event_t<> on_restore;
		atma::evented::event_t<> on_close;

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
