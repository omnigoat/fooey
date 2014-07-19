#pragma once
//======================================================================
#include <atma/types.hpp>
//======================================================================
namespace fooey
{
	enum class key_t : uint8
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
}
