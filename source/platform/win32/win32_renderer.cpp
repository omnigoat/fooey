#include <fooey/fooey.hpp>
#include <atma/assert.hpp>

#include <atma/config/platform.hpp>

#include <map>
#include <thread>

using namespace fooey;


//======================================================================
// win32_renderer_t
//======================================================================
struct win32_renderer_t : fooey::renderer_t
{
	win32_renderer_t();
	~win32_renderer_t();

	auto register_window(window_ptr const& window) -> void override;

private:
	auto build_win32_window(window_ptr const&) -> HWND;

	std::thread wndproc_thread_;

	typedef std::map<HWND, widget_ptr> mapped_hwnds_t;
	mapped_hwnds_t mapped_hwnds_;
};

static LRESULT CALLBACK wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//BaseWindow *c = (BaseWindow *)GetWindowLong(hWnd, GWLP_USERDATA);

	//if (c == NULL)
	return DefWindowProc(hWnd, msg, wParam, lParam);

	//return c->WindowProc(hWnd, msg, wParam, lParam);
}


//======================================================================
// system_renderer()
//======================================================================
win32_renderer_t::win32_renderer_t()
{
	wndproc_thread_ = std::thread([] {
		
	});
}

win32_renderer_t::~win32_renderer_t()
{
	wndproc_thread_.join();
}

auto fooey::system_renderer() -> fooey::renderer_ptr
{
	return fooey::renderer_ptr(new win32_renderer_t);
}




//======================================================================
// win32_renderer_t implementation
//======================================================================
auto win32_renderer_t::register_window(window_ptr const& window) -> void
{
	HWND hwnd = build_win32_window(window);

	mapped_hwnds_[hwnd] = window;
}


auto win32_renderer_t::build_win32_window(window_ptr const& window) -> HWND
{
	static char const* win32_classnames = "abcdefghijklmno";
	static uint32_t win32_classname_idx = 0;

	HINSTANCE hh = GetModuleHandle(NULL);

	ATMA_ASSERT(win32_classnames[win32_classname_idx]);

	WNDCLASS wc = {
		0, &wnd_proc,
		0, 0,
		hh,
		nullptr,
		nullptr,
		(HBRUSH)(COLOR_BACKGROUND),
		nullptr,
		win32_classnames + win32_classname_idx++
	};

	ATOM class_atom = RegisterClass(&wc);
	ATMA_ASSERT(class_atom);

	HWND hwnd = CreateWindow((LPCTSTR)class_atom, window->caption().c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0,0,640,480,0,0,hh,NULL);
	return hwnd;
}


#if 0


#if 0
namespace
{
	
}
#endif


using fooey::window_t;

window_t::window_t(properties::captioned_t::caption_t const& caption)
	: properties::property_t(false), captioned_t(caption)
{
#if 0
	
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