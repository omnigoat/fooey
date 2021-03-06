#pragma once

#include <fooey/widget.hpp>
#include <fooey/properties.hpp>
#include <fooey/input/key_state.hpp>

#include <atma/event.hpp>
#include <atma/lockfree/queue.hpp>
#include <atma/config/platform.hpp>
#include <atma/thread/engine.hpp>


namespace fooey
{
	enum class window_state_t
	{
		windowed,
		fullscreen,
		maximized,
		minimized
	};

	struct window_t :
		widget_t,
		properties::captioned_t
	{
		window_t(properties::captioned_t::caption_t const& caption, uint32 width, uint32 height);
		~window_t();

		auto set_visible(bool) -> void;
		auto signal(atma::thread::engine_t::signal_t const& fn) -> void { engine_.signal(fn); }
		auto signal_block() -> void { engine_.signal_block(); }

		auto state() const -> window_state_t { return window_state_; }
		auto drawcontext_width() const -> uint32 { return dc_width_; }
		auto drawcontext_height() const -> uint32 { return dc_height_; }

		//
		key_state_t key_state;
		bool fullscreen_;
		atma::thread::engine_t engine_;
		window_state_t window_state_;
		uint32 dc_width_;
		uint32 dc_height_;
	};

	typedef std::shared_ptr<window_t> window_ptr;


	inline auto window(atma::string const& caption, uint32 width, uint32 height) -> window_ptr
	{
		return window_ptr(new window_t(caption, width, height));
	}
}
