#pragma once

#include <fooey/fooey_fwd.hpp>
#include <fooey/widget.hpp>
#include <fooey/widgets/window.hpp>

#include <memory>

namespace fooey
{
	struct renderer_t
	{
		virtual ~renderer_t() {}
		virtual auto add_window(window_ptr const&) -> void = 0;
	};

	// creates a system-renderer
	auto system_renderer() -> renderer_ptr;
}
