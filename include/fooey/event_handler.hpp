#pragma once
//=====================================================================
#include <atma/assert.hpp>
#include <atma/string.hpp>
#include <atma/enable_multiple_shared_from_this.hpp>
#include <atma/utf/utf8_string_range.hpp>
#include <atma/xtm/function.hpp>

#include <fooey/event.hpp>

#include <functional>
#include <vector>
#include <map>
#include <memory>
#include <tuple>
#include <mutex>
//=====================================================================
namespace fooey
{
	enum class event_traversal_t
	{
		none,
		upwards,
		downwards,
		up_down
	};

	struct event_handler_t : virtual atma::enable_multiple_shared_from_this
	{
		struct namedesc_t;
		struct homogenized_function_t;
		
		typedef std::multimap<namedesc_t, homogenized_function_t> mapped_fns_t;
		typedef std::vector<mapped_fns_t::const_iterator> delegate_set_t;

	public:
		virtual ~event_handler_t() {}

		auto on(std::initializer_list<std::pair<atma::string, homogenized_function_t>> bindings) -> delegate_set_t;
		auto unbind(delegate_set_t const&) -> void;

		template <typename T = event_t>
		auto fire(atma::string const& name, T& e, event_traversal_t t = event_traversal_t::upwards) -> void
		{
			std::lock_guard<std::mutex> LG(mutex_);

			fire_impl(name, static_cast<event_t&>(e));

			if (t == event_traversal_t::upwards)
			{
				if (e.propagate() && parent_handler_)
					parent_handler_->fire(name, e, t);
			}
			else if (t == event_traversal_t::downwards)
			{
				for (auto const& x : children_handlers_)
				{
					ATMA_ASSERT(x);
					x->fire(name, e, t);
				}
			}
		}

		template <typename T = event_t>
		auto fire(atma::string const& name) -> void
		{
			fire(name, event_t(shared_from_this<widget_t>()), event_traversal_t::upwards);
		}


	protected:
		event_handler_t();
		event_handler_t(event_handler_t*);

		auto add_child_handler(event_handler_t*) -> void;
		auto set_parent_handler(event_handler_t*) -> void;

	private:
		typedef std::function<void(event_t&)> fn_t;
		
		auto fire_impl(atma::string const&, event_t&) -> void;
		auto insert(namedesc_t const&, fn_t const&) -> void;
		
		mapped_fns_t mapped_fns_;
		event_handler_t* parent_handler_;
		typedef std::vector<event_handler_t*> children_t;
		children_t children_handlers_;
		std::mutex mutex_;
	};


	struct event_handler_t::namedesc_t
	{
		namedesc_t(atma::string const&);
		namedesc_t(namedesc_t const&);
		auto operator = (namedesc_t const&) -> void = delete;

		// if we match rhs, but we're allowed to have extraneous classes
		auto containing_match(namedesc_t const&) const -> bool;

	private:
		atma::string base_;
		atma::utf8_string_range_t id_;
		std::vector<atma::utf8_string_range_t> classes_;
		
		friend auto operator < (namedesc_t const& lhs, namedesc_t const& rhs) -> bool;

		friend struct event_handler_t;
	};


	struct event_handler_t::homogenized_function_t
	{
		// functions that take no arguments are allowed, we just ignore the event_t argument
		template <typename FN, typename std::enable_if<atma::xtm::arity_equal_to<FN, 0>::value, int>::type = 0>
		homogenized_function_t(FN const& fn)
		{
			fn_ = [fn](event_t&) -> void {
				fn();
			};
		}

		// any function that takes one argument is assumed to take an event that inherits from event_t
		template <typename FN, typename std::enable_if<atma::xtm::arity_equal_to<FN, 1>::value, int>::type = 0>
		homogenized_function_t(FN const& fn)
		{
			typedef std::decay_t<typename atma::xtm::function_traits<FN>::arg<0>::type> T2;

			fn_ = [fn](event_t& e) -> void
			{
				ATMA_ASSERT_MSG(!!dynamic_cast<T2*>(&e), "bad event type");
				fn(static_cast<T2&>(e));
			};
		}

		auto operator()(event_t& e) -> void {
			return fn_(e);
		}

	private:
		typedef std::function<void(event_t&)> fn_t;

		fn_t fn_;
	};

}
