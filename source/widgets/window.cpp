#include <atma/xtm/function.hpp>
#include <atma/xtm/tuple.hpp>
#include <string>
#include <tuple>


#include <fooey/widgets/window.hpp>

using namespace fooey;
using fooey::window_t;
using fooey::window_ptr;



window_t::window_t(properties::captioned_t::caption_t const& c)
	: captioned_t(c)
{
}

auto fooey::map_basic_window_events(window_ptr const& wnd) -> void
{
}




#if 0

auto blah(int, float&, std::string const&) -> void {
	auto f = float();
	auto const p = std::tuple<int, float&, std::string>(4, f, std::string("4"));
	atma::xtm::apply_tuple(&blah, p);
}

#endif

