#include <fooey/fooey.hpp>
#include <atma/assert.hpp>

#include <atma/config/platform.hpp>
#include <atma/lockfree/queue.hpp>
#include <fooey/events/resize.hpp>
#include <fooey/events/move.hpp>
#include <fooey/events/mouse.hpp>
#include <map>
#include <thread>

using namespace fooey;

namespace
{
	using events::resizing_edge;

	WPARAM resizing_edge_to_wparam[] =
	{
		0, WMSZ_LEFT, WMSZ_RIGHT, 0,
		WMSZ_TOP, WMSZ_TOPLEFT, WMSZ_TOPRIGHT, 0,
		WMSZ_BOTTOM, WMSZ_BOTTOMLEFT, WMSZ_BOTTOMRIGHT, 0
	};

	resizing_edge wparam_to_resizing_edge[] =
	{
		resizing_edge::none,
		resizing_edge::left,
		resizing_edge::right,
		resizing_edge::top,
		resizing_edge::top_left,
		resizing_edge::top_right,
		resizing_edge::bottom,
		resizing_edge::bottom_left,
		resizing_edge::bottom_right
	};
}


//======================================================================
// 
//======================================================================
struct win32_root_widget_t : fooey::widget_t
{
	win32_root_widget_t()
	{
	}
};


//======================================================================
// win32_renderer_t
//======================================================================
struct win32_renderer_t : fooey::renderer_t
{
	win32_renderer_t();
	~win32_renderer_t();

	auto add_window(window_ptr const& window) -> void override;

private:
	auto build_win32_window(window_ptr const&) -> HWND;

	std::thread wndproc_thread_;

	typedef atma::lockfree::queue_t< std::function<void()> > commands_t;
	commands_t commands_;
	bool running_;

	typedef std::map<HWND, widget_ptr> mapped_hwnds_t;
	mapped_hwnds_t mapped_hwnds_;

	widget_ptr root_widget_;
};



struct userdata_t
{
	widget_wptr widget;
	bool mouse_entered;
};


LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// get widget pointer from longptr data
	auto userdata = (userdata_t*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	auto const& widget_weak = userdata->widget;
	auto widget = widget_weak.lock();
	auto window = std::dynamic_pointer_cast<window_t>(widget);
	ATMA_ASSERT(widget);


	switch (msg)
	{
		case WM_SYSCOMMAND:
		{
			switch (wparam)
			{
				case SC_CLOSE:
					window->fire("close");
					break;
			}
			break;
		}

		case WM_MOVE:
			window->fire("move", events::move_t(widget_weak, LOWORD(lparam), HIWORD(lparam)));
			break;

		case WM_SIZING:
			window->fire("resize", events::resize_t(widget_weak, wparam_to_resizing_edge[wparam], (LPRECT)lparam));
			break;

		case WM_SIZE:
		{
			int32_t width = LOWORD(lparam);
			int32_t height = HIWORD(lparam);

			std::cout << "WM_SIZE [" << wparam << "] " << width << "x" << height << std::endl;

			// since WM_SIZING doesn't get sent for maximize/minimize events, but we still need to update
			// our window's stored size, we'll grab the size and send an internal size event
			switch (wparam)
			{
				case SIZE_MAXIMIZED:
				{
					window->fire("resize.internal", events::resize_t(widget_weak, resizing_edge::none, width, height));
					window->fire("maximized");
					break;
				}

				case SIZE_MINIMIZED:
				{
					window->fire("resize.internal", events::resize_t(widget_weak, resizing_edge::none, width, height));
					window->fire("minimized");
					break;
				}

				case SIZE_RESTORED:
				{
					window->fire("resize.internal", events::resize_t(widget_weak, resizing_edge::none, width, height));
					window->fire("restored");
					break;
				}
			}

			window->fire("resize-dc", events::resize_t(widget_weak, resizing_edge::none, LOWORD(lparam), HIWORD(lparam)));
			break;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			window->key_state.down(static_cast<fooey::key_t>(wparam));
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			window->key_state.up(static_cast<fooey::key_t>(wparam));
			break;

		case WM_MOUSEMOVE:
			if (!userdata->mouse_entered) {
				window->fire("mouse-entered", events::mouse_t(widget_weak, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));
				userdata->mouse_entered = true;
				TRACKMOUSEEVENT tme{sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 0};
				TrackMouseEvent(&tme);
			}
			else {
				window->fire("mouse-move", events::mouse_t(widget_weak, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));
			}
			break;

		case WM_MOUSELEAVE:
			userdata->mouse_entered = false;
			window->fire("mouse-leave");
			break;

		case WM_LBUTTONDOWN:
			window->fire("mouse-down.left", events::mouse_t(widget_weak, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));
			break;

		case WM_LBUTTONUP:
			window->fire("mouse-up.left", events::mouse_t(widget_weak, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));
			break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK wnd_proc_setup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// on window creation, set the longptr handles for user-data, and
	// use the *actual* wndproc after we safely know the user-data will be present.
	if (msg == WM_CREATE)
	{
		auto weak_widget_raw = (widget_wptr*)((CREATESTRUCT*)lparam)->lpCreateParams;
		ATMA_ASSERT(weak_widget_raw);
		auto weak_widget = *weak_widget_raw;
		auto widget = weak_widget.lock();
		ATMA_ASSERT(widget);
		auto g = new userdata_t{weak_widget, false};
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)g);

		auto window = std::dynamic_pointer_cast<window_t>(widget);
		ATMA_ASSERT(window);
		window->set_hwnd(hwnd);

		RECT rect, r2;
		GetWindowRect(hwnd, &rect);
		GetClientRect(hwnd, &r2);

		window->fire("move.internal", events::move_t(weak_widget, rect.left, rect.top));
		window->fire("resize-dc.internal", events::resize_t(weak_widget, resizing_edge::none, r2.right - r2.left, r2.bottom - r2.top));

		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)&wnd_proc);
		//SetWindowLongPtr(hwnd, GWLP_USERDATA, )
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//======================================================================
// system_renderer()
//======================================================================
win32_renderer_t::win32_renderer_t()
: root_widget_(new win32_root_widget_t), running_(true)
{
}

win32_renderer_t::~win32_renderer_t()
{
}

auto fooey::system_renderer() -> fooey::renderer_ptr
{
	return fooey::renderer_ptr(new win32_renderer_t);
}

//======================================================================
// win32_renderer_t implementation
//======================================================================
auto win32_renderer_t::add_window(window_ptr const& window) -> void
{
	HWND hwnd = build_win32_window(window);
	
	root_widget_->add_child(window);
}


auto win32_renderer_t::build_win32_window(window_ptr const& window) -> HWND
{

	// create win32 window in the thread-engine of the window
	window->signal([=] {
		static wchar_t const* win32_classnames = L"abcdefghijklmno";
		static uint32 win32_classname_idx = 0;

		HINSTANCE hh = GetModuleHandle(NULL);

		ATMA_ASSERT(win32_classnames[win32_classname_idx]);

		auto wc = WNDCLASSW{
			0, &wnd_proc_setup,
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

		// dynamically allocate a weak_ptr for this window
		auto wptr = new widget_wptr(window);

		wchar_t SERIOUSLY_buffer[256];
		auto p = atma::utf16_from_utf8(SERIOUSLY_buffer, window->caption().raw_begin(), window->caption().raw_end());
		*p = '\0';

		HWND hwnd = CreateWindow((LPCWSTR)class_atom, SERIOUSLY_buffer, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, window->width_in_pixels(), window->height_in_pixels(), 0, 0, hh,
			wptr);

		ATMA_ENSURE(hwnd);
	});

	window->signal_block();

	// make the window always process the message loop!
	window->engine_.signal_evergreen([] {
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, TRUE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	});

	return 0;
}
