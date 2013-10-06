#include <fooey/event.hpp>
#include <algorithm>

using namespace fooey;
using fooey::event_handler_t;

event_handler_t::event_handler_t()
: parent_handler_()
{
}

event_handler_t::event_handler_t(event_handler_t* parent_handler)
: parent_handler_(parent_handler)
{
}

auto event_handler_t::add_child_handler(event_handler_t* child) -> void
{
	return children_handlers_.push_back(child);
}

auto event_handler_t::set_parent_handler(event_handler_t* parent) -> void
{
	parent_handler_ = parent;
}

auto event_handler_t::insert(namedesc_t const& desc, fn_t const& fn) -> void
{
	mapped_fns_.insert({desc, fn});
}

auto event_handler_t::fire_impl(std::string const& name, event_t& e) -> void
{
	auto N = namedesc_t(name);
	auto R = mapped_fns_.equal_range(N);

	for (auto i = R.first; i != R.second; ++i)
	{
		auto const& X = i->first;

		if (std::includes(X.classes_.begin(), X.classes_.end(), N.classes_.begin(), N.classes_.end(),
			[](namedesc_t::section_t const& lhs, namedesc_t::section_t const& rhs) -> bool {
				return std::lexicographical_compare(lhs.first, lhs.second, rhs.first, rhs.second);
			}))
			i->second(e);
	}
}

event_handler_t::namedesc_t::namedesc_t(std::string const& str)
	: base_(str)
{
	// id
	{
		auto i = base_.begin();
		for (; i != base_.end(); ++i)
			if (*i == '.')
				break;
		id_ = section_t(base_.begin(), i);
	}

	// classes
	{
		auto c0 = base_.find_first_of('.', 0);
		if (c0 == std::string::npos)
			return;
		
		while (c0 != std::string::npos)
		{
			auto c1 = base_.find_first_of('.', c0 + 1);
			if (c1 == std::string::npos) {
				c1 = base_.size();
				classes_.push_back({base_.begin() + c0 + 1, base_.begin() + c1});
				break;
			}
			classes_.push_back({base_.begin() + c0 + 1, base_.begin() + c1});
			c0 = c1;
		}

		std::sort(classes_.begin(), classes_.end());
	}
}

event_handler_t::namedesc_t::namedesc_t(namedesc_t const& rhs)
	: base_(rhs.base_)
{
	id_.first = base_.begin() + std::distance(rhs.base_.begin(), rhs.id_.first);
	id_.second = base_.begin() + std::distance(rhs.base_.begin(), rhs.id_.second);

	for (auto const& x : rhs.classes_)
		classes_.push_back({
			base_.begin() + std::distance(rhs.base_.begin(), x.first),
			base_.begin() + std::distance(rhs.base_.begin(), x.second)
		});
}

auto fooey::operator < (event_handler_t::namedesc_t const& lhs, event_handler_t::namedesc_t const& rhs) -> bool
{
	auto lhsd = std::distance(lhs.id_.first, lhs.id_.second);
	auto rhsd = std::distance(rhs.id_.first, rhs.id_.second);

	if (lhsd < rhsd)
		return true;
	else if (rhsd < lhsd)
		return false;

	auto ib = strncmp(&*lhs.id_.first, &*rhs.id_.first, lhsd);
	if (ib < 0)
		return true;
	else if (0 < ib)
		return false;

	return false;
}


fooey::event_t::~event_t() {}