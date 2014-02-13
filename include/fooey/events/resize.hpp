#ifndef FOOEY_EVENTS_RESIZE_HPP
#define FOOEY_EVENTS_RESIZE_HPP
//======================================================================
#include <atma/config/platform.hpp>
//======================================================================
namespace fooey {
namespace events {
//======================================================================
	
	enum class resizing_edge : uint32
	{
		none = 0,

		left = 1,
		right = 2,
		top = 4,
		bottom = 8,

		top_left = top | left,
		top_right = top | right,
		bottom_left = bottom | left,
		bottom_right = bottom | right
	};

//======================================================================
} // namespace events
} // namespace fooey
//======================================================================
#ifdef ATMA_PLATFORM_WIN32
#	include <fooey/platform/win32/events/resize.hpp>
#endif
//======================================================================
#endif // include guard
//======================================================================
