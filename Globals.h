#pragma once
#include <Windows.h>



namespace Structs
{
	struct glow_t {
		DWORD dwBase;
		float r;
		float g;
		float b;
		float m_flGlowAlpha;
		char m_unk[4];
		float m_flUnk;
		float m_flBloomAmount;
		float m_flUnk1;
		bool m_bRenderWhenOccluded;
		bool m_bRenderWhenUnoccluded;
		bool m_bFullBloomRender;
		char m_unk1;
		int m_nFullBloomStencilTestValue;
		int m_nGlowStyle;
		int m_nSplitScreenSlot;
		int m_nNextFreeSlot;
	};

	typedef struct {
		float x, y, z;
	}Vector;

	struct Color
	{
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE a;
	};

}

namespace G
{
	namespace Toggles
	{
		bool bToggle = false;
		bool tToggle = false;
		bool fToggle = false;
		bool gToggle = false;
		bool vgToggle = false;
		bool aToggle = false;
		bool rToggle = false;
		bool cToggle = false;
		bool radToggle = false;
		bool fovToggle = false;
	};
	namespace Colors
	{
		Structs::Color glowCol;
		Structs::Color vglowCol;
		Structs::Color chamCol;
	}

}
