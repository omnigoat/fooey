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
	};


	
	struct event_handler_t
	{
		struct namedesc_t;

		template <typename FN>
		auto on(std::string const& name, FN fn) -> void
		{
			insert(namedesc_t(name), homogenize(fn));
		}

		template <typename T>
		auto fire(std::string const& name, T& e) -> void
		{
			fire_impl(name, static_cast<event_t&>(e));
		}

	private:
		typedef std::function<void(event_t&)> fn_t;
		

		template <typename T>
		static auto homogenize(T fn) -> fn_t
		{
			return [fn](event_t& e) -> void {
				typedef typename std::decay_t<typename atma::xtm::function_traits<T>::arg<0>::type> T2;
				auto nep = dynamic_cast<T2*>(&e);
				ATMA_ASSERT(nep);
				fn(*nep);
			};
		}

		auto fire_impl(std::string const&, event_t&) -> void;

		auto insert(namedesc_t const&, fn_t const&) -> void;
		
		std::multimap<namedesc_t, fn_t> mapped_fns_;
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
