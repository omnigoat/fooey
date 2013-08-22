#include <fooey/widget.hpp>
#include <fooey/fooey.hpp>

using namespace fooey;

using fooey::widget_t;
using fooey::widget_ptr;
using fooey::widget_group_t;




//======================================================================
// widget_t
//======================================================================
widget_t::widget_t()
{
}

widget_t::~widget_t()
{
}

auto widget_t::queued_events() -> event_queue_t&
{
	return event_queue_;
}

auto widget_t::queue_event(widget_t::queued_event_t::first_type time, widget_t::queued_event_t::second_type msg) -> void
{
	event_queue_.push(std::make_pair(time, msg));
}

auto fooey::operator , (widget_ptr const& lhs, widget_ptr const& rhs) -> widget_group_t
{
	return widget_group_t{lhs, rhs};
}



//======================================================================
// widget_group_t
//======================================================================
widget_group_t::widget_group_t()
{
}

widget_group_t::widget_group_t( std::initializer_list<widget_ptr> elements )
	: elements_(elements.begin(), elements.end())
{
}

widget_group_t::widget_group_t(widget_group_t&& rhs)
	: elements_( std::move(rhs.elements_) )
{
}

auto fooey::operator , (widget_group_t& lhs, widget_ptr const& rhs) -> widget_group_t&
{
	lhs.elements_.push_back(rhs);
	return lhs;
}

