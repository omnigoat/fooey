#ifndef FOOEY_HPP
#define FOOEY_HPP
//======================================================================
#include <string>
#include <set>
#include <vector>
//======================================================================
namespace fooey {
//======================================================================
	
	struct ux_t
	{
		enum type_t {
			percentage,
			exact
		};

		//auto type() const -> type_t { return type_; }
		//auto value() const -> float { return value_; }

	//private:
		type_t type;
		float value;
		//void* relative_to_;
	};


	// a widget is an on-screen element that is extensible.
	// a.k.a: it is not for things which are merely concepts. concrete things only please.
	struct widget_t;
	typedef std::shared_ptr<widget_t> widget_ptr;
	typedef std::weak_ptr<widget_t> widget_wptr;

	
	struct widget_t
	{
		widget_t();
		virtual ~widget_t();

	private:
		widget_wptr parent_;
		ux_t left_, top_, width_, height_;
	};

	

	struct containing_widget_t : widget_t
	{
		typedef std::set<widget_ptr> children_t;

		auto add_child(widget_ptr const&) -> void;
		
	private:
		children_t children_;
	};


	

	

	


	namespace properties
	{
		enum class event_t
		{
			caption_changed,
		};


		//======================================================================
		// a property!
		//======================================================================
		struct property_t
		{
			

			property_t(bool propagate_changes = true)
				: propagate_changes_(propagate_changes)
			{
			}

			auto trigger_change(event_t e) -> void {
				if (propagate_changes_)
					on_changed_property(e);
			}

			auto propagate_changes(bool k) -> void {
				propagate_changes_ = k;
			}

		protected:
			virtual auto on_changed_property(event_t) -> void = 0;

		private:
			bool propagate_changes_;
		};


		//======================================================================
		// captioned
		//======================================================================
		struct captioned_t : virtual property_t
		{
			typedef std::string caption_t;

			captioned_t(caption_t const& caption)
				: caption_(caption)
			{
			}

			auto caption() const -> caption_t const& {
				return caption_;
			}
		
			auto set_caption(caption_t const& caption) -> void { 
				caption_ = caption;
				trigger_change(event_t::caption_changed);
			}

		private:
			caption_t caption_;
		};
	}




	//======================================================================
	// window
	//======================================================================
	struct window_t : containing_widget_t, properties::captioned_t
	{
		window_t(properties::captioned_t::caption_t const& caption)
			: properties::property_t(false), captioned_t(caption)
		{
		}
		
		auto set_visible(bool) -> void;

	private:
		auto on_input_event(input::event_t e) -> void //override
		{
		}

		auto on_changed_property(properties::event_t e) -> void override
		{
			switch (e)
			{
				case properties::event_t::caption_changed:
					// do something?
					break;

				default:
					break;
			}
		}
	};


	struct button_t : widget_t, properties::captioned_t
	{

	};

//======================================================================
} // fooey
//======================================================================
#endif
//======================================================================
