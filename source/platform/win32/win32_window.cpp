#if 0
#include <fooey/fooey.hpp>
#include <atma/assert.hpp>

#include <atma/config/platform.hpp>

#include <map>

#if 0
namespace
{
	typedef std::map<HWND, void*> mapped_hwnds_t;
	mapped_hwnds_t mapped_hwnds_;
}
#endif

using namespace fooey;

using fooey::window_t;

window_t::window_t(properties::captioned_t::caption_t const& caption)
	: properties::property_t(false), captioned_t(caption)
{
#if 0
	WNDCLASS wc = {
		0,
		wnd_proc,
		0,
		0,
		hInstance,
		nullptr,
		nullptr,
		(HBRUSH)(COLOR_BACKGROUND),
		nullptr,
		"some class"
	};

	ATOM class_atom = RegisterClass(&wc);
	ATMA_ASSERT(class_atom);

	HWND hwnd = 
		CreateWindow(class_atom, caption.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0,0,640,480,0,0,hInstance,NULL);
	
	mapped_hwnds_[hwnd] = this;

	//while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
		//DispatchMessage( &msg );
#endif
}

auto window_t::on_changed_property(properties::event_t e) -> void
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
#endif