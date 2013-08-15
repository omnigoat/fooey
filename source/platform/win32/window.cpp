#include <fooey/fooey.hpp>
#include <atma/assert.hpp>

//#include <atma/config/platform.hpp>
#include <Windows.h>


using namespace fooey;

using fooey::window_t;

window_t::window_t(properties::captioned_t::caption_t const& caption)
	: properties::property_t(false), captioned_t(caption)
{
	MSG msg;
	WNDCLASS wc;
	
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hInstance;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = L"minwindowsapp";

	ATOM class_atom = RegisterClass(&wc);
	ATMA_ASSERT(class_atom);

	HWND hwnd = 
		CreateWindow(class_atom, caption.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0,0,640,480,0,0,hInstance,NULL);
	
	while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
		DispatchMessage( &msg );

	return 0;
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