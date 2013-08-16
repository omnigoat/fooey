#ifndef FOOEY_WIDGETS_WINDOW_HPP
#define FOOEY_WIDGETS_WINDOW_HPP
//======================================================================
#include <fooey/widget.hpp>
#include <fooey/properties.hpp>
//======================================================================
namespace fooey {
//======================================================================
	
	//======================================================================
	// window
	//======================================================================
	struct window_t : 
		contiaining_widget_t,
		properties::captioned_t
	{
		window_t(properties::captioned_t::caption_t const& caption);
		
		auto set_visible(bool) -> void;

	private:
		auto on_input_event(input::event_t e) -> void;

		auto on_changed_property(properties::event_t e) -> void override;
	};

//======================================================================
} // fooey
//======================================================================
#endif
//======================================================================
