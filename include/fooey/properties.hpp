#ifndef FOOEY_PROPERTIES_HPP
#define FOOEY_PROPERTIES_HPP
//======================================================================
#include <memory>
#include <string>
#include <set>
#include <vector>
//======================================================================
namespace fooey {
namespace properties {
//======================================================================
	
	enum class event_t
	{
		caption_changed,
	};


	//======================================================================
	// property_t
	//   - inherited virtually
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
		//atma::lockfree::queue_t<event_t> event_queue_;
		bool propagate_changes_;
	};




	//======================================================================
	// captioned
	//======================================================================
	struct captioned_t //: virtual property_t
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
			//trigger_change(event_t::caption_changed);
		}

	private:
		caption_t caption_;
	};

//======================================================================
} // properties
} // fooey
//======================================================================
#endif
//======================================================================
