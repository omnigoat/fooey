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

