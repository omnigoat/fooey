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

	struct resize_t : event_t
	{
		resize_t(widget_wptr const& origin, resizing_edge edge, LPRECT rect)
		: event_t(origin), edge_(edge),
		  width_(rect->right - rect->left), height_(rect->bottom - rect->top)
		{
		}

		resize_t(widget_wptr const& origin, resizing_edge edge, int32_t width, int32_t height)
			: event_t(origin), edge_(edge),
			width_(width), height_(height)
		{
		}

		auto edge() const -> resizing_edge { return edge_; }

		auto width() const -> int32_t { return width_; }
		auto height() const -> int32_t { return height_; }

	private:
		resizing_edge edge_;
		int32_t width_, height_;
	};
	
//======================================================================
} // namespace events
} // namesapce fooey
//======================================================================
#endif
//======================================================================
