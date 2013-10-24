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
		: event_t(origin), edge_(edge), rect_(rect)
		{
		}

		auto rect() const -> LPRECT { return rect_; }
		auto edge() const -> resizing_edge { return edge_; }

		auto left() const -> uint32_t { return rect_->left; }
		auto right() const -> uint32_t { return rect_->right; }
		auto top() const -> uint32_t { return rect_->top; }
		auto bottom() const -> uint32_t { return rect_->bottom; }

		auto width() const -> uint32_t { return rect_->right - rect_->left; }
		auto height() const -> uint32_t { return rect_->bottom - rect_->top; }

	private:
		resizing_edge edge_;
		LPRECT rect_;
	};
	
//======================================================================
} // namespace events
} // namesapce fooey
//======================================================================
#endif
//======================================================================
