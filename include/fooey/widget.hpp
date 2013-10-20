#ifndef FOOEY_WIDGET_HPP
#define FOOEY_WIDGET_HPP
//======================================================================
#include <memory>
#include <string>
#include <set>
#include <vector>
#include <initializer_list>
#include <chrono>
#include <atma/lockfree/queue.hpp>
#include <fooey/event.hpp>
#include <atma/intrusive_ptr.hpp>
//======================================================================
namespace fooey {
//======================================================================
	
	struct widget_t;
	typedef std::shared_ptr<widget_t> widget_ptr;
	typedef std::weak_ptr<widget_t> widget_wptr;

//======================================================================
} // namespace fooey
//======================================================================
#include <fooey/platform/win32/widget.hpp>
//======================================================================
namespace fooey {
//======================================================================
	
	// forward declares
	struct widget_group_t;
	template <typename> struct widget_builder_t;

	struct ux_t
	{
		enum type_t {
			px,
			pc,
			pt
		};

		type_t type;
		float value;

		ux_t() : type(), value() {}
	};

	template <typename R, typename C, typename... Args> void match_fn(R (C::*)(Args...)) {}
	template <typename R, typename C, typename... Args> void match_fn_const(R (C::*)(Args...) const) {}

	inline void match_widget()
	{
		match_fn_const(&widget_t::children);

		match_fn(&widget_t::add_child);

		static_assert( std::is_base_of<std::enable_shared_from_this<widget_t>, widget_t>::value, 
			"widget_t needs to derive from enable_shared_from_this" );
	}


	

	//======================================================================
	// widget_builder_t
	// ------------------
	//======================================================================
	template <typename T>
	struct widget_builder_t
	{
		widget_builder_t()
		{
		}

		explicit widget_builder_t(T* x)
		: backend_(x)
		{
		}

		template <typename U>
		explicit widget_builder_t(std::shared_ptr<U> const& rhs)
		: backend_(std::dynamic_pointer_cast<T>(rhs))
		{
		}

		widget_builder_t(widget_builder_t const& rhs)
		: backend_(rhs.backend_)
		{
		}

		template <typename U>
		widget_builder_t(widget_builder_t<U> const& rhs)
		: backend_(rhs.backend_)
		{
		}

		widget_builder_t(widget_builder_t&& rhs)
		: backend_(std::move(rhs.backend_))
		{
		}

		auto operator * () const -> widget_builder_t& { return *backend_; }
		auto operator -> () const -> T* { return backend_.get(); }


		auto operator [] (widget_ptr const&) -> widget_ptr&
		{
			backend_->add_child(child);
			return *this;
		}

		auto operator[](widget_group_t const&)->widget_ptr&
		{
			for (auto const& x : group.elements())
				backend_->add_child(x);
			return *this;
		}

		operator bool()
		{
			return !!backend_;
		}

		auto backend() const -> std::shared_ptr<T> const& { return backend_; }
		auto get() const -> T* { return backend_.get(); }

		template <typename Y>
		operator std::shared_ptr<Y>() const
		{
			return std::static_pointer_cast<Y>(backend_);
		}

	private:
		std::shared_ptr<T> backend_;

		template <typename U>
		friend struct widget_builder_t;
	};

	template <typename T>
	inline auto operator < (widget_builder_t<T> const& lhs, widget_builder_t<T> const& rhs) -> bool
	{
		return lhs.get() < rhs.get();
	}


	//======================================================================
	// widget_group_t
	// ----------------
	//======================================================================
	struct widget_group_t
	{
		typedef std::vector<widget_ptr> elements_t;

		widget_group_t();
		widget_group_t( std::initializer_list<widget_ptr> );
		widget_group_t(widget_group_t&&);
		
		auto elements() const -> elements_t const& { return elements_; }

	private:
		elements_t elements_;

		friend auto operator , (widget_group_t& lhs, widget_ptr const&) -> widget_group_t&;
	};

	auto operator , (widget_group_t&, widget_ptr const&) -> widget_group_t&;

	auto operator , (widget_ptr const& lhs, widget_ptr const& rhs) -> widget_group_t;

	
	struct resize_event_t : event_t
	{
		resize_event_t(widget_wptr const& target, uint32_t width, uint32_t height)
			: width_(width), height_(height)
		{
		}

		auto target() const -> widget_wptr { return target_; }
		auto width() const -> uint32_t { return width_; }
		auto height() const -> uint32_t { return height_; }

	private:
		widget_wptr target_;
		uint32_t width_, height_;
	};


	

//======================================================================
} // fooey
//======================================================================
#endif
//======================================================================
