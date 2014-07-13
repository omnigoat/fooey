#include <fooey/keys.hpp>
#include <algorithm>

using namespace fooey;

using fooey::key_state_t;

auto key_state_t::down(key_t k) -> void
{
	uint8_t ki = static_cast<uint8_t>(k);

	// if we're already down, ignore
	if (bitfield_[ki])
		return;

	bitfield_[ki] = true;

	// find all events that use this key as a trigger
	events_t es;
	for (auto& e : down_events_)
	{
		key_sequence_t const& seq = std::get<0>(e);
		uint32& chord = std::get<2>(e);

		auto nr = seq.combination(chord).bitfield() & bitfield_;
		//auto knr = nr

		if (nr.at(ki))
		//if (chord_partial_match(seq.combination(chord).bitfield()))
		{
			//if (chord_match(seq.combination(chord).bitfield()))
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

auto key_state_t::on_key(key_t k, std::function<void()> const& fn) -> uint32
{
	down_events_.push_back(std::make_tuple(fooey::key_sequence_t(k), fn, 0u));
	return (uint32)down_events_.size();
}

auto key_state_t::on_key(key_combination_t const& k, std::function<void()> const& fn) -> uint32
{
	down_events_.push_back(std::make_tuple(fooey::key_sequence_t(k), fn, 0u));
	return (uint32)down_events_.size();
}

auto key_state_t::up(key_t k) -> void
{
	uint8_t ki = static_cast<uint8_t>(k);

	// if we're already up, ignore
	if (!bitfield_[ki])
		return;

	// find all events that use this key as a trigger
	events_t es;
	for (auto& e : up_events_)
	{
		key_sequence_t const& seq = std::get<0>(e);
		uint32& chord = std::get<2>(e);

		auto nr = seq.combination(chord).bitfield() & bitfield_;
		//auto knr = nr

		if (nr.at(ki))
		//if (chord_partial_match(seq.combination(chord).bitfield()))
		{
			//if (chord_match(seq.combination(chord).bitfield()))
				es.push_back(e);
		}
		else {
			chord = 0;
		}
	}

	bitfield_[ki] = false;


	for (auto const& e : es)
	{
		std::get<1>(e)();
	}
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

auto key_state_t::on_key_up(key_t k, std::function<void()> const& fn)->uint32
{
	up_events_.push_back(std::make_tuple(fooey::key_sequence_t(k), fn, 0u));
	return (uint32)up_events_.size();
}
