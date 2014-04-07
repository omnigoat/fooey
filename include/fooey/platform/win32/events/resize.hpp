#ifndef FOOEY_PLATFORM_WIN32_EVENTS_RESIZE_EVENT_HPP
#define FOOEY_PLATFORM_WIN32_EVENTS_RESIZE_EVENT_HPP
//======================================================================
#include <vector>
#include <memory>
#include <fooey/event.hpp>
//======================================================================
namespace fooey {
namespace events {
//======================================================================
	
	enum class resize_action_t
	{
		restore,
		minimize,
		maximize,
		fullscreen
	};

	struct resize_t : event_t
	{
		resize_t(widget_wptr const& origin, resizing_edge edge, LPRECT rect, bool fullscreen = false)
		: event_t(origin), edge_(edge),
		  width_(rect->right - rect->left), height_(rect->bottom - rect->top),
		  fullscreen_(fullscreen)
		{
		}

		resize_t(widget_wptr const& origin, resizing_edge edge, int32_t width, int32_t height, bool fullscreen = false)
			: event_t(origin), edge_(edge),
			width_(width), height_(height),
			fullscreen_(fullscreen)
		{
		}

		auto edge() const -> resizing_edge { return edge_; }

		auto width() const -> int32_t { return width_; }
		auto height() const -> int32_t { return height_; }

		auto fullscreen() const -> bool { return fullscreen_; }

	private:
		resizing_edge edge_;
		int32_t width_, height_;
		bool fullscreen_;
	};
	
//======================================================================
} // namespace events
} // namesapce fooey
//======================================================================
#endif
//======================================================================
