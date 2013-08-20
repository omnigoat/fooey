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

	auto register_window(window_ptr const& window) -> void override;

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
#if 0
	switch (msg)
	{
		case WM_SYSCOMMAND:
		{
			switch (wparam)
			{
				case SW_MINIMIZE:
					window->queue_event(&window_t::on_minimise);
					break;

				case SW_MAXIMIZE:
					window->queue_event(&window_t::on_maximise);
					break;

				case SC_CLOSE:
					window->queue_event(&window_t::on_close);
					break;
			}
			break;
		}
	}
#endif

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
auto win32_renderer_t::register_window(window_ptr const& window) -> void
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

		HWND hwnd = CreateWindow((LPCTSTR)class_atom, window->caption().c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0,0,640,480,0,0,hh,NULL);
	});

	return 0;
}
