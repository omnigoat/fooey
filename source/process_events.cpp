#include <fooey/fooey.hpp>

#include <algorithm>

using namespace fooey;

typedef std::tuple<
	widget_t::queued_event_t::first_type,
	widget_t::queued_event_t::second_type,
	widget_ptr> bound_event_t;

typedef std::vector<bound_event_t> bound_events_t;

auto process_events_impl(bound_events_t& events, widget_ptr const& x) -> void
{
	widget_t::queued_event_t e;
	while (x->queued_events().pop(e)) {
		events.push_back(std::make_tuple(e.first, e.second, x));
	}

	// recurse
	for (auto const& child : x->children())
		process_events_impl(events, x);
}

auto fooey::process_events(widget_ptr const& x) -> void
{
	// get all events
	bound_events_t events;
	process_events_impl(events, x);

	// sort based on time of arrival
	std::sort(events.begin(), events.end());

	// do things
	for (auto const& x : events)
	{
		switch (std::get<1>(x))
		{
			// window-only events
			case event_t::minimise:
				std::static_pointer_cast<fooey::window_t>(std::get<2>(x).get_backend())->on_minimise.fire();
				break;

			case event_t::maximise:
				std::static_pointer_cast<fooey::window_t>(std::get<2>(x).get_backend())->on_maximise.fire();
				break;

			case event_t::restore:
				std::static_pointer_cast<fooey::window_t>(std::get<2>(x).get_backend())->on_restore.fire();
				break;

			case event_t::close:
				std::static_pointer_cast<fooey::window_t>(std::get<2>(x).get_backend())->on_close.fire();
				break;
		}
	}
}