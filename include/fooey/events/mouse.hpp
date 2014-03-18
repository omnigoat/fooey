#pragma once
//======================================================================
#include <fooey/event.hpp>
//======================================================================
namespace fooey { namespace events {
	
	struct mouse_t : event_t
	{
		mouse_t(event_handler_wptr const& origin, int x, int y)
			: event_t(origin), x_(x), y_(y)
		{}

		auto x() const -> int { return x_; }
		auto y() const -> int { return y_; }

	private:
		int x_, y_;
	};

} }

