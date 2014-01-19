#include <fooey/fooey.hpp>
#include <atma/assert.hpp>

#include <atma/config/platform.hpp>
#include <atma/lockfree/queue.hpp>
#include <fooey/events/resize.hpp>
#include <fooey/events/move.hpp>
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

	events::resizing_edge wparam_to_resizing_edge[] =
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


LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// get widget pointer from longptr data
	auto widget_raw = (widget_wptr*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	ATMA_ASSERT(widget_raw);
	auto widget_weak = *widget_raw;
	auto widget = widget_weak.lock();
	auto window = std::dynamic_pointer_cast<window_t>(widget);
	ATMA_ASSERT(widget);


	atma::event_flow_t fc;

	switch (msg)
	{
		case WM_SYSCOMMAND:
		{

			switch (wparam)
			{
				case SC_MINIMIZE:
					window->fire("minimise");
					break;

				case SC_MAXIMIZE:
					std::cout << "maximise!" << std::endl;
					window->fire("maximise");
					break;

				case SC_RESTORE:
					window->fire("restore");
					break;

				case SC_CLOSE:
					window->fire("close");
					return 0;
			}
			break;
		}

		/*
		case WM_WINDOWPOSCHANGING: {
			if (window->fullscreen_) {
				auto wndpos = (LPWINDOWPOS)lparam;
				wndpos->flags |= SWP_NOSIZE;
			}
			break;
		}
		*/

		case WM_MOVE:
			window->fire("move", events::move_t(widget_weak, LOWORD(lparam), HIWORD(lparam)));
			break;

		case WM_SIZING: {
			auto rect = (LPRECT)lparam;
			//std::cout << "WM_SIZING " << rect->left << ":" << rect->top << " " << (rect->right - rect->left) << "x" << (rect->bottom - rect->top) << std::endl;
			window->fire("resize", events::resize_t(widget_weak, wparam_to_resizing_edge[wparam], (LPRECT)lparam));
			break;
		}

		case WM_SIZE:
		{
			std::cout << "WM_SIZE [" << wparam << "] " << LOWORD(lparam) << "x" << HIWORD(lparam) << std::endl;
			window->fire("resize-dc", events::resize_t(widget_weak, resizing_edge::none, LOWORD(lparam), HIWORD(lparam)));
			break;
		}


		case WM_SYSKEYDOWN:
			window->key_state.down(static_cast<fooey::key_t>(wparam));
			break;

		case WM_SYSKEYUP:
			window->key_state.up(static_cast<fooey::key_t>(wparam));
			break;

		case WM_KEYDOWN:
			window->key_state.down(static_cast<fooey::key_t>(wparam));
			break;

		case WM_KEYUP:
			window->key_state.up(static_cast<fooey::key_t>(wparam));
			break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK wnd_proc_setup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// on window creation, set the longptr handles for user-data, and
	// use the *actual* wndproc after we safely know the user-data will be present.
	if (msg == WM_CREATE) {
		
		auto weak_widget_raw = (widget_wptr*)((CREATESTRUCT*)lparam)->lpCreateParams;
		ATMA_ASSERT(weak_widget_raw);
		auto weak_widget = *weak_widget_raw;
		auto widget = weak_widget.lock();
		ATMA_ASSERT(widget);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)weak_widget_raw);

		auto window = std::dynamic_pointer_cast<window_t>(widget);
		ATMA_ASSERT(window);
		window->set_hwnd(hwnd);

		RECT rect;
		GetWindowRect(hwnd, &rect);

		window->fire("move.internal", events::move_t(weak_widget, rect.left, rect.top));
		
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG)&wnd_proc);
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//======================================================================
// system_renderer()
//======================================================================
win32_renderer_t::win32_renderer_t()
: root_widget_(new win32_root_widget_t), running_(true)
{
#if 0
	wndproc_thread_ = std::thread([=] {
		while (running_)
		{
			// first, do commands!
			std::function<void()> command;
			while (commands_.pop(command)) {
				command();
			}

			// secondly, do messages
			// ...
			MSG msg;
			while (PeekMessage(&msg, NULL, 0, 0, TRUE) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// do any more commands that were left over
		std::function<void()> command;
		while (commands_.pop(command)) {
			command();
		}
	});
#endif
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
		static char const* win32_classnames = "abcdefghijklmno";
		static uint32_t win32_classname_idx = 0;

		HINSTANCE hh = GetModuleHandle(NULL);

		ATMA_ASSERT(win32_classnames[win32_classname_idx]);

		auto wc = WNDCLASS{
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

		HWND hwnd = CreateWindow((LPCTSTR)class_atom, window->caption().c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, window->width_in_pixels(), window->height_in_pixels(), 0, 0, hh,
			wptr);

		ATMA_ENSURE(hwnd);
	});

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
