#ifndef FOOEY_PLATFORM_WIN32_EVENTS_MOVE_EVENT_HPP
#define FOOEY_PLATFORM_WIN32_EVENTS_MOVE_EVENT_HPP
//======================================================================
#include <vector>
#include <memory>
#include <fooey/event.hpp>
//======================================================================
namespace fooey {
namespace events {
//======================================================================

	struct move_t : event_t
	{
		move_t(widget_wptr const& origin, int32_t left, int32_t top)
			: event_t(origin),
			left_(left), top_(top)
		{
		}

		auto left() const -> int32_t { return left_; }
		auto top() const -> int32_t { return top_; }

	private:
		int32_t left_, top_;
	};
	
//======================================================================
} // namespace events
} // namesapce fooey
//======================================================================
#endif
//======================================================================
