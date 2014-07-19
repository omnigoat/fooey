#pragma once
//======================================================================
#include <atma/types.hpp>

#include <cstdint>
#include <vector>
#include <functional>
#include <bitset>
//======================================================================
namespace fooey
{
	enum class key_t : uint8;




	struct key_combination_t
	{
		typedef std::bitset<128> bitfield_t;

		key_combination_t(key_t k);
		key_combination_t(std::initializer_list<key_t> keys);

		auto bitfield() const -> bitfield_t const&;

	private:
		bitfield_t keys_;
	};

	auto operator + (key_t lhs, key_t rhs) -> key_combination_t;




	struct key_sequence_t
	{
		typedef std::vector<key_combination_t> combinations_t;

		key_sequence_t(key_t);
		key_sequence_t(std::initializer_list<key_combination_t>);
		key_sequence_t(key_combination_t const&);

		auto combination(uint i) const -> key_combination_t const&;
		auto chord_count() const -> uint;

	private:
		combinations_t combinations_;
	};




	struct key_state_t
	{
		typedef std::bitset<128> bitfield_t;

		auto down(key_t) -> void;
		auto up(key_t) -> void;

		auto on_key_down(key_sequence_t const&, std::function<void()> const&) -> uint;
		auto on_key_up  (key_sequence_t const&, std::function<void()> const&) -> uint;

	private:
		auto perform_on_down_actions() -> void;
		auto chord_match(bitfield_t const&) -> bool;
		auto chord_partial_match(bitfield_t const&) -> bool;

	private:
		typedef std::tuple<key_sequence_t, std::function<void()>, uint> event_t;
		typedef std::vector<event_t> events_t;

		bitfield_t bitfield_;
		events_t down_events_, up_events_;
	};
}
