#include <fooey/fooey.hpp>
#include <atma/assert.hpp>

#include <atma/config/platform.hpp>
#include <atma/lockfree/queue.hpp>

#include <map>
#include <thread>

using namespace fooey;

//======================================================================
// 
//======================================================================



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
};

static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//auto now = std::chrono::high_resolution_clock::now();

	auto widget = (widget_t*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	auto window = dynamic_cast<window_t*>(widget);
	
	atma::event_flow_t fc;

	switch (msg)
	{
		case WM_CREATE:
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lparam)->lpCreateParams);
			break;

		case WM_SYSCOMMAND:
		{
			switch (wparam)
			{
				case SC_MINIMIZE:
					fc = window->on_minimise.fire();
					break;

				case SC_MAXIMIZE:
					fc = window->on_maximise.fire();
					break;

				case SC_RESTORE:
					fc = window->on_restore.fire();
					break;

				case SC_CLOSE:
					fc = window->on_close.fire();
					break;
			}
			break;
		}

		case WM_SIZE:
			fc = window->on_resize.fire((uint32_t)(lparam & 0xffff), (uint32_t)(lparam >> 16));
			break;

		case WM_KEYDOWN:
			window->key_state.down(static_cast<fooey::key_t>(wparam));
			break;

		case WM_KEYUP:
			window->key_state.up(static_cast<fooey::key_t>(wparam));
			break;
	}

	if (fc.is_prevented())
		return 0;
	
	return DefWindowProc(hwnd, msg, wparam, lparam);
}



//======================================================================
// system_renderer()
//======================================================================
win32_renderer_t::win32_renderer_t()
	: running_(true)
{
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
}

win32_renderer_t::~win32_renderer_t()
{
	running_ = false;
	wndproc_thread_.join();
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

	mapped_hwnds_[hwnd] = window;
}


auto win32_renderer_t::build_win32_window(window_ptr const& window) -> HWND
{
	commands_.push([&window] {
		static char const* win32_classnames = "abcdefghijklmno";
		static uint32_t win32_classname_idx = 0;

		HINSTANCE hh = GetModuleHandle(NULL);

		ATMA_ASSERT(win32_classnames[win32_classname_idx]);

		auto wc = WNDCLASS{
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



		HWND hwnd = CreateWindow((LPCTSTR)class_atom, window->caption().c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0,0,window->width_in_pixels(),window->height_in_pixels(),0,0,hh, window.get());
		window->hwnd = hwnd;
	});

	return 0;
}
