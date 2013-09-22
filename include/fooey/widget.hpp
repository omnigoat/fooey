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
//======================================================================
namespace fooey {
//======================================================================
	
	struct ux_t
	{
		enum type_t {
			percentage,
			exact
		};

		type_t type;
		float value;

		ux_t() : type(), value() {}
	};


	// a widget is an on-screen element that is extensible.
	// a.k.a: it is not for things which are merely concepts. concrete things only please.
	struct widget_t;
	
	struct widget_group_t;
	template <typename> struct widget_ptr_t;
	typedef widget_ptr_t<widget_t> widget_ptr;
	typedef std::weak_ptr<widget_t> widget_wptr;

	struct widget_event_t
	{
		widget_wptr target;
	};


	//======================================================================
	// basic widget
	//======================================================================
	struct widget_t
	{
		typedef std::vector<widget_ptr> children_t;
		//typedef std::pair<std::chrono::high_resolution_clock::time_point, event_t> queued_event_t;
		//typedef atma::lockfree::queue_t<queued_event_t> event_queue_t;

		widget_t();
		widget_t(uint32_t width, uint32_t height);
		virtual ~widget_t();

		auto width_in_pixels() const -> uint32_t { return width_; }
		auto height_in_pixels() const -> uint32_t { return height_; }
		auto children() const -> children_t const& { return children_; }
		//auto queued_events() -> event_queue_t&;

		auto add_child(widget_ptr const&) -> void;
		//auto queue_event(std::chrono::high_resolution_clock::time_point, event_t) -> void;

	protected:
		widget_t* parent_;
		children_t children_;
		uint32_t left_, top_, width_, height_;

	private:
		// events
		//event_queue_t event_queue_;
	};

	auto operator , (widget_ptr const& lhs, widget_ptr const& rhs) -> widget_group_t;
	
	template <typename T>
	struct widget_ptr_t
	{
		widget_ptr_t()
		{
		}

		explicit widget_ptr_t(T* x)
		: backend_(x)
		{
		}

		template <typename U>
		explicit widget_ptr_t(std::shared_ptr<U> const& rhs)
		: backend_(std::dynamic_pointer_cast<T>(rhs))
		{
		}

		widget_ptr_t(widget_ptr_t const& rhs)
		: backend_(rhs.backend_)
		{
		}

		template <typename U>
		widget_ptr_t(widget_ptr_t<U> const& rhs)
		: backend_(rhs.backend_)
		{
		}

		widget_ptr_t(widget_ptr_t&& rhs)
		: backend_(std::move(rhs.backend_))
		{
		}

		auto operator = (widget_ptr_t const& rhs)-> widget_ptr_t&
		{
			backend_ = rhs.backend_;
			return *this;
		}

		auto operator = (widget_ptr_t&& rhs) -> widget_ptr_t&
		{
			std::swap(backend_, rhs.backend_);
			return *this;
		}
		
		auto operator * () const -> widget_ptr_t& { return *backend_; }
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

	private:
		std::shared_ptr<T> backend_;

		template <typename U>
		friend struct widget_ptr_t;
	};

	template <typename T>
	inline auto operator < (widget_ptr_t<T> const& lhs, widget_ptr_t<T> const& rhs) -> bool
	{
		return lhs.get() < rhs.get();
	}


	//======================================================================
	// used for building structure
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



	


	

//======================================================================
} // fooey
//======================================================================
#endif
//======================================================================
