#pragma once

namespace fooey
{
	struct widget_t;
	typedef std::shared_ptr<widget_t> widget_ptr;
	typedef std::weak_ptr<widget_t> widget_wptr;

	struct window_t;
	typedef std::shared_ptr<window_t> window_ptr;
}