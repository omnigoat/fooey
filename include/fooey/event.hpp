//=====================================================================
//
//
//
//=====================================================================
#ifndef FOOEY_EVENT_HPP
#define FOOEY_EVENT_HPP
//=====================================================================
#include <atma/assert.hpp>
#include <atma/lockfree/queue.hpp>
#include <atma/xtm/function.hpp>
#include <mutex>
#include <functional>
#include <vector>
#include <set>
#include <map>
//=====================================================================
namespace fooey {
//=====================================================================
	
	// 
	struct event_t
	{
		virtual ~event_t();
		auto is_propagating() const -> bool { return propagate_; }
		auto set_propagating(bool b) -> void { propagate_ = b; }

	private:
		bool propagate_;
	};


	enum class event_traversal_t
	{
		none,
		upwards,
		downwards,
		up_down
	};

	
	struct event_handler_t
	{
		struct namedesc_t;

		template <typename FN>
		auto on(std::string const& name, FN fn) -> void
		{
			insert(namedesc_t(name), homogenize(fn));
		}

		template <typename T = event_t>
		auto fire(std::string const& name, T& e = event_t(), event_traversal_t t = event_traversal_t::upwards) -> void
		{
			fire_impl(name, static_cast<event_t&>(e));

			if (t == event_traversal_t::upwards)
			{
				if (e.is_propagating() && parent_handler_)
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


	protected:
		event_handler_t();
		event_handler_t(event_handler_t*);

		auto add_child_handler(event_handler_t*) -> void;
		auto set_parent_handler(event_handler_t*) -> void;

	private:
		typedef std::function<void(event_t&)> fn_t;
		

		template <typename T>
		static auto homogenize(T fn) -> fn_t
		{
			return [fn](event_t& e) -> void {
				typedef typename std::decay_t<typename atma::xtm::function_traits<T>::arg<0>::type> T2;
				auto nep = dynamic_cast<T2*>(&e);
				ATMA_ASSERT_MSG(nep, "bad event type");
				fn(*nep);
			};
		}

		auto fire_impl(std::string const&, event_t&) -> void;

		auto insert(namedesc_t const&, fn_t const&) -> void;
		
		std::multimap<namedesc_t, fn_t> mapped_fns_;
		event_handler_t* parent_handler_;
		typedef std::vector<event_handler_t*> children_t;
		children_t children_handlers_;
	};

	struct event_handler_t::namedesc_t
	{
		namedesc_t(std::string const&);
		namedesc_t(namedesc_t const&);
		auto operator = (namedesc_t const&) -> void = delete;

	private:
		typedef std::pair<std::string::const_iterator, std::string::const_iterator> section_t;

		std::string base_;
		section_t id_;
		std::vector<section_t> classes_;

		friend auto operator < (namedesc_t const& lhs, namedesc_t const& rhs) -> bool;

		friend struct event_handler_t;
	};

	

//=====================================================================
} // namespace fooey
//=====================================================================
#endif // inclusion guard
//=====================================================================
