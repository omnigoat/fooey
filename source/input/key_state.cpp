#include <fooey/input/key_state.hpp>

#include <fooey/keys.hpp>

#include <algorithm>


using namespace fooey;
using fooey::key_combination_t;
using fooey::key_sequence_t;
using fooey::key_state_t;



key_combination_t::key_combination_t(key_t k)
{
	keys_[static_cast<uint8_t>(k)] = true;
}

key_combination_t::key_combination_t(std::initializer_list<key_t> keys)
{
	for (auto const x : keys)
		keys_[static_cast<uint8_t>(x)] = true;
}

auto key_combination_t::bitfield() const -> bitfield_t const&
{
	return keys_;
}


auto fooey::operator + (key_t lhs, key_t rhs) -> key_combination_t
{
	return key_combination_t{lhs, rhs};
}


key_sequence_t::key_sequence_t(key_t k)
{
	combinations_.push_back(key_combination_t(k));
}

key_sequence_t::key_sequence_t(std::initializer_list<key_combination_t> combinations)
: combinations_(combinations.begin(), combinations.end())
{
}

key_sequence_t::key_sequence_t(key_combination_t const& c)
{
	combinations_.push_back(c);
}

auto key_sequence_t::combination(uint i) const -> key_combination_t const&
{
	return combinations_[i];
}

auto key_sequence_t::chord_count() const -> uint
{
	return (uint)combinations_.size();
}





auto key_state_t::down(key_t k) -> void
{
	uint8 ki = static_cast<uint8>(k);

	if (bitfield_[ki])
		return;

	bitfield_[ki] = true;

	// trigger all events waiting upon this key combination
	for (auto& e : down_events_)
	{
		key_sequence_t const& seq = std::get<0>(e);
		uint& chord = std::get<2>(e);
		auto const& combination_bitfield = seq.combination(chord).bitfield();

		auto matching_event = bitfield_ & combination_bitfield;
		if (combination_bitfield.test(ki) && matching_event == combination_bitfield)
		{
			++chord;
			if (chord == seq.chord_count()) {
				std::get<1>(e)();
				chord = 0;
			}
		}
		else {
			chord = 0;
		}
	}
}

auto key_state_t::up(key_t k) -> void
{
	uint8 ki = static_cast<uint8>(k);

	if (!bitfield_.test(ki))
		return;

	// trigger all events waiting upon this key combination
	for (auto& e : up_events_)
	{
		key_sequence_t const& seq = std::get<0>(e);
		uint& chord = std::get<2>(e);
		auto const& combination_bitfield = seq.combination(chord).bitfield();

		auto matching_event = bitfield_ & combination_bitfield;
		if (combination_bitfield.test(ki) && matching_event == combination_bitfield)
		{
			++chord;
			if (chord == seq.chord_count()) {
				std::get<1>(e)();
				chord = 0;
			}
		}
		else {
			chord = 0;
		}
	}

	bitfield_[ki] = false;
}

auto key_state_t::on_key_down(key_sequence_t const& k, std::function<void()> const& fn) -> uint
{
	down_events_.push_back(std::make_tuple(k, fn, 0u));
	return (uint)down_events_.size();
}

auto key_state_t::on_key_up(key_sequence_t const& k, std::function<void()> const& fn)->uint
{
	up_events_.push_back(std::make_tuple(k, fn, 0u));
	return (uint)up_events_.size();
}
