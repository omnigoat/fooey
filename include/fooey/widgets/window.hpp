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

		//
		key_state_t key_state;
	};

	typedef std::shared_ptr<window_t> window_ptr;


	inline auto window(std::string const& caption, uint32_t width, uint32_t height) -> window_ptr
	{
		return window_ptr(new window_t(caption, width, height));
	}


//======================================================================
} // fooey
//======================================================================
#endif
//======================================================================
