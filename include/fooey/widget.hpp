#ifndef FOOEY_HPP
#define FOOEY_HPP
//======================================================================
#include <memory>
#include <string>
#include <set>
#include <vector>
#include <initializer_list>
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
	struct widget_ptr;
	
	struct widget_group_t;
	
	
	//======================================================================
	// basic widget
	//======================================================================
	struct widget_t
	{
		widget_t();
		virtual ~widget_t();

		auto operator [] (widget_ptr const&) -> widget_ptr;
		auto operator [] (widget_group_t const&) -> widget_ptr;

	private:
		widget_t* parent_;
		children_t children_;

		ux_t left_, top_, width_, height_;
	};

	auto operator , (widget_ptr const& lhs, widget_ptr const& rhs) -> widget_group_t;
	

	struct widget_ptr
	{
		explicit widget_ptr(widget_t*);
		widget_ptr(widget_ptr const&);
		widget_ptr(widget_ptr&&);

		auto operator = (widget_ptr const&) -> widget_ptr&;
		auto operator = (widget_ptr&&) -> widget_ptr&;
		
		auto operator * () const -> widget_t&;
		auto operator -> () const -> widget_t*;

	private:
		std::shared_ptr<widget_t> backend_;
	};



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
