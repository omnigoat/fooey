#pragma once
//=====================================================================
#include <fooey/fooey_fwd.hpp>
//=====================================================================
namespace fooey
{
	struct event_t
	{
		event_t(event_handler_wptr const& origin)
			: origin_(origin)
		{}

		virtual ~event_t() {}

		auto propagate() const -> bool { return propagate_; }
		auto origin() const -> event_handler_wptr { return origin_; }

		auto disable_propagation() -> void { propagate_ = false; }

	private:
		mutable bool propagate_ = true;
		event_handler_wptr origin_;
	};
}
