#pragma once
//======================================================================
#include <atma/types.hpp>

#include <cstdint>
#include <vector>
#include <functional>
#include <bitset>
//======================================================================
namespace fooey {

	enum class key_t : uint8;

	struct key_combination_t
	{
		typedef std::bitset<128> bitfield_t;

		key_combination_t(key_t k)
		{
			keys_[static_cast<uint8_t>(k)] = true;
		}

		key_combination_t(std::initializer_list<key_t> keys)
		{
			for (auto const x : keys)
				keys_[static_cast<uint8_t>(x)] = true;
		}

		auto bitfield() const -> bitfield_t const& { return keys_; }

	private:
		bitfield_t keys_;
	};

	inline auto operator + (key_t lhs, key_t rhs) -> key_combination_t
	{
		return key_combination_t{lhs, rhs};
	}

#if 0
	inline auto operator & (key_combination_t const& lhs, key_t rhs) -> key_combination_t
	{
		return key_combination_t(lhs.bitfield() & static_cast<uint8_t>(rhs));
	}
#endif

	struct key_sequence_t
	{
		typedef std::vector<key_combination_t> combinations_t;

		key_sequence_t(key_t k)
		{
			combinations_.push_back(key_combination_t(k));
		}

		key_sequence_t(std::initializer_list<key_combination_t> combinations)
			: combinations_(combinations.begin(), combinations.end())
		{
		}

		key_sequence_t(key_combination_t const& c)
		{
			combinations_.push_back(c);
		}

		auto combination(uint32 i) const -> key_combination_t const&
		{
			return combinations_[i];
		}

	private:
		combinations_t combinations_;
	};



	struct key_state_t
	{
		typedef std::bitset<128> bitfield_t;

		auto down(key_t) -> void;
		auto up(key_t) -> void;

		auto on_key_down(key_combination_t const&, std::function<void()> const&)->uint32;
		auto on_key_up(key_combination_t const&, std::function<void()> const&)->uint32;
		
	private:
		auto perform_on_down_actions() -> void;
		auto chord_match(bitfield_t const&) -> bool;
		auto chord_partial_match(bitfield_t const&) -> bool;

	private:
		typedef std::tuple<key_sequence_t, std::function<void()>, uint32> event_t;
		typedef std::vector<event_t> events_t;

		bitfield_t bitfield_;
		events_t down_events_, up_events_;
	};
}
