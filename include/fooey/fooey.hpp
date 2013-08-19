#ifndef FOOEY_HPP
#define FOOEY_HPP
//======================================================================
#include <memory>
#include <string>
#include <set>
#include <vector>
#include <atma/evented/event.hpp>
#include <fooey/widget.hpp>
#include <fooey/widgets/window.hpp>
//======================================================================
namespace fooey {
//======================================================================
	
	struct window_event_t
	{
		enum id_t {
			minimise,
			maximise,
			restore,
			close
		};
	};



	struct renderer_t
	{
		virtual ~renderer_t() {}
		virtual auto register_window(window_ptr const&) -> void = 0;
	};

	typedef std::shared_ptr<renderer_t> renderer_ptr;

	auto system_renderer() -> renderer_ptr;

	namespace input
	{
		auto register_ui(widget_ptr const&) -> void;
	}

//======================================================================
} // fooey
//======================================================================
#endif
//======================================================================
