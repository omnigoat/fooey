#pragma once
//======================================================================
#include <atma/types.hpp>

#include <cstdint>
#include <vector>
#include <functional>
#include <bitset>
//======================================================================
namespace fooey {
//======================================================================

	enum class key_t : uint8_t
	{
		// control
		Tab = 0x09,
		Enter = 0x0d,
		Shift = 0x10,
		Ctrl = 0x11,
		Alt = 0x12,
		Esc = 0x1b,
		Left = 0x25,
		Up = 0x26,
		Right = 0x27,
		Down = 0x28,

		// letters
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4a,
		K = 0x4b,
		L = 0x4c,
		M = 0x4d,
		N = 0x4e,
		O = 0x4f,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5a,
	};
	

	//
	// what is a key-combination
	//  - one or more keys
	//  - a sequence of one or more keys
	


	

	struct key_combination_t
	{
		typedef std::bitset<256> bitfield_t;

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
		typedef std::bitset<256> bitfield_t;

		auto down(key_t) -> void;
		auto up(key_t) -> void;

		auto on_key(key_t, std::function<void()> const&) -> uint32;
		auto on_key_up(key_t, std::function<void()> const&) -> uint32;
		auto on_key(key_combination_t const&, std::function<void()> const&)->uint32;
		
		//auto on_key_combination(uint8_t, uint8_t, std::function<void()> const&)->uint32;

	private:
		auto perform_on_down_actions() -> void;
		auto chord_match(bitfield_t const&) -> bool;
		auto chord_partial_match(bitfield_t const&) -> bool;

		typedef std::bitset<256> bitfield_t;
		//typedef std::pair<key_sequence_t, std::function<void()>> event_t;
		typedef std::tuple<key_sequence_t, std::function<void()>, uint32> event_t;
		typedef std::vector<event_t> events_t;

		bitfield_t bitfield_;
		events_t down_events_, up_events_;
	};

//======================================================================
} // fooey
//======================================================================
