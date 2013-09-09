#include <fooey/keys.hpp>
#include <algorithm>

using namespace fooey;

using fooey::key_state_t;

auto key_state_t::down(key_t k) -> void
{
	uint8_t ki = static_cast<uint8_t>(k);

	bitfield_[ki] = true;

	// find all events that use this key as a trigger
	events_t es;
	for (auto& e : down_events_)
	{
		key_sequence_t const& seq = std::get<0>(e);
		uint32_t& chord = std::get<2>(e);

		if (chord_partial_match(seq.combination(chord).bitfield()))
		{
			if (chord_match(seq.combination(chord).bitfield()))
				es.push_back(e);
		}
		else {
			chord = 0;
		}
	}

	for (auto const& e : es)
	{
		std::get<1>(e)();
	}
}

auto key_state_t::on_key(key_t k, std::function<void()> const& fn) -> uint32_t
{
	down_events_.push_back(std::make_tuple(fooey::key_sequence_t(k), fn, 0u));
	return down_events_.size();
}

auto key_state_t::on_key(key_combination_t const& k, std::function<void()> const& fn) -> uint32_t
{
	down_events_.push_back(std::make_tuple(fooey::key_sequence_t(k), fn, 0u));
	return down_events_.size();
}

auto key_state_t::up(key_t k) -> void
{
	bitfield_[static_cast<uint8_t>(k)] = false;
}

auto key_state_t::chord_match(bitfield_t const& rhs) -> bool
{
	// bits in the bitfield that are set that aren't required in rhs
	auto nr = (bitfield_ ^ rhs) & bitfield_;

	return nr.none() && (bitfield_ ^ ~rhs).all();
}

auto key_state_t::chord_partial_match(bitfield_t const& rhs) -> bool
{
	// bits in the bitfield that are set that aren't required in rhs
	auto nr = (bitfield_ ^ rhs) & bitfield_;
	
	return nr.none() && (bitfield_ & rhs).any();
}