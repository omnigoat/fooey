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
		  left_(rect->left), right_(rect->right), top_(rect->top), bottom_(rect->bottom)
		{
		}

		auto edge() const -> resizing_edge { return edge_; }

		auto left() const -> int32_t { return left_; }
		auto right() const -> int32_t { return right_; }
		auto top() const -> int32_t { return top_; }
		auto bottom() const -> int32_t { return bottom_; }

		auto width() const -> int32_t {
			return right_ - left_;
		}

		auto height() const -> int32_t {
			return bottom_ - top_;
		}

	private:
		resizing_edge edge_;
		int32_t left_, right_, top_, bottom_;
	};
	
//======================================================================
} // namespace events
} // namesapce fooey
//======================================================================
#endif
//======================================================================
