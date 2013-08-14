#ifndef FOOEY_HPP
#define FOOEY_HPP
//======================================================================
#include <atma/string.hpp>
#include <string>
//======================================================================
namespace fooey {
//======================================================================
	
	struct ux_t
	{
		enum {
			relative,
			absolute,
		}

		enum {
			
			percentage,

		};
	};

	// a widget is an on-screen element that is extensible.
	// a.k.a: it is not for things which are merely concepts. concrete
	// things only please.
	struct widget_t
	{
		widget_t();
		virtual ~widget_t();

	private:
		// logical position and dimensions
		float left_, top_, width_, height_;
	};

	typedef std::shared_ptr<widget_t> widget_ptr;
	

	struct containing_widget_t : widget_t
	{
		typedef std::set<widget_ptr> children_t;

		auto add_child(widget_ptr const&) -> void;
		
	private:
		children_t children_;
	};


	struct window_t : containing_widget_t
	{
		window_t();
		
		auto set_visible(bool) -> void;

	private:
	};


	struct button_t : widget_t
	{
		
	private:
		std::string caption_;
	};

//======================================================================
} // fooey
//======================================================================
#endif
//======================================================================
