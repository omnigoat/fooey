#include <fooey/event.hpp>
#include <fooey/event_handler.hpp>
#include <algorithm>
#include <atma/string.hpp>

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
	std::lock_guard<std::mutex> LG(mutex_);
	return children_handlers_.push_back(child);
}

auto event_handler_t::set_parent_handler(event_handler_t* parent) -> void
{
	std::lock_guard<std::mutex> LG(mutex_);
	parent_handler_ = parent;
}

auto event_handler_t::insert(namedesc_t const& desc, fn_t const& fn) -> void
{
	std::lock_guard<std::mutex> LG(mutex_);
	mapped_fns_.insert({desc, fn});
}

auto event_handler_t::fire_impl(atma::string const& name, event_t& e) -> void
{
	auto N = namedesc_t(name);
	auto R = mapped_fns_.equal_range(N);

	for (auto i = R.first; i != R.second; ++i)
	{
		auto const& x = i->first;

		if ( x.containing_match(N) )
			i->second(e);
	}
}

auto event_handler_t::on(std::initializer_list<std::pair<atma::string, homogenized_function_t>> bindings) -> delegate_set_t
{
	std::lock_guard<std::mutex> LG(mutex_);

	delegate_set_t set;
	for (auto const& x : bindings)
		set.push_back(mapped_fns_.insert(std::make_pair(namedesc_t(x.first), x.second)));
	return set;
}

auto event_handler_t::unbind(delegate_set_t const& xs) -> void
{
	std::lock_guard<std::mutex> LG(mutex_);

	for (auto const& x : xs)
		mapped_fns_.erase(x);
}

event_handler_t::namedesc_t::namedesc_t(atma::string const& str)
	: base_(str)
{
	// id
	{
		id_ = atma::utf8_string_range_t(
			base_.begin(),
			atma::find_first_of(base_, '.'));
	}

	// classes
	{
		auto c0 = atma::find_first_of(base_, '.');
		if (c0 == base_.end())
			return;
		
		while (c0 != base_.end())
		{
			++c0;
			auto c1 = atma::find_first_of(base_, c0, '.');
			classes_.push_back({c0, c1});
			c0 = c1;
		}

		std::sort(classes_.begin(), classes_.end());
	}
}

event_handler_t::namedesc_t::namedesc_t(namedesc_t const& rhs)
	: base_(rhs.base_)
{
	id_ = atma::rebase_string_range(base_, rhs.base_, rhs.id_);

	for (auto const& x : rhs.classes_)
		classes_.push_back( rebase_string_range(base_, rhs.base_, x) );
}

auto fooey::event_handler_t::namedesc_t::containing_match(namedesc_t const& rhs) const -> bool
{
	return std::includes(classes_.begin(), classes_.end(), rhs.classes_.begin(), rhs.classes_.end());
}


auto fooey::operator < (event_handler_t::namedesc_t const& lhs, event_handler_t::namedesc_t const& rhs) -> bool
{
	return lhs.id_ < rhs.id_;
}


