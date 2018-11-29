#include "MemManager.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include "offsets.h"
MemManager* m;
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


Color glowCol;
Color vglowCol;
Color chamCol;
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

int defaultFOV;
void radar()
{
	if (GetAsyncKeyState(VK_F10))
	{
		radToggle = !radToggle;
		if (radToggle) std::cout << "Radar ON" << std::endl;
		else std::cout << "Radar OFF" << std::endl;
		Sleep(200);
	}
	if (!radToggle) return;
	DWORD dwPla = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	int lTeamNum = m->ReadMem<int>(dwPla + offsets::teamNum);
	for (int i = 1; i < 64; i++)
	{
		int pEntity = m->ReadMem<int>(m->cDll.dwBase + offsets::entityList + i * 0x10);
		int pEntTeam = m->ReadMem<int>(pEntity + offsets::teamNum);
		bool dormant = m->ReadMem<int>(pEntity + offsets::bDormant);

		if (pEntTeam == lTeamNum) continue;
		if (dormant) continue;

		m->WriteMem<int>(pEntity + offsets::m_bSpotted, 1);
		Sleep(1);
	}
}

void chams()
{
	if (GetAsyncKeyState(VK_F9))
	{
		cToggle = !cToggle;
		if (cToggle) std::cout << "Chams ON" << std::endl;
		else std::cout << "Chams OFF" << std::endl;
		Sleep(200);
	}
	Color obj;
	if (!cToggle) return;
	DWORD dwPla = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	int lTeamNum = m->ReadMem<int>(dwPla + offsets::teamNum);
	for (int i = 1; i < 64; i++)
	{
		int pEntity = m->ReadMem<int>(m->cDll.dwBase + offsets::entityList + i * 0x10);
		int pEntTeam = m->ReadMem<int>(pEntity + offsets::teamNum);
		bool dormant = m->ReadMem<int>(pEntity + offsets::bDormant);

		if (pEntTeam == lTeamNum) continue;
		if (dormant) continue;

		obj.r = 45;
		obj.g = 4;
		obj.b = 86;
		obj.a = 255;

		m->WriteMem<Color>(pEntity + 0x70, obj);
		Sleep(1);
	}
}

void RCS()
{
	if (GetAsyncKeyState(VK_F8))
	{
		rToggle = !rToggle;
		if (rToggle) std::cout << "RCS ON" << std::endl;
		else std::cout << "RCS OFF" << std::endl;
		Sleep(200);
	}
	if (!rToggle) return;

	DWORD ClientState;
	DWORD pLocalPlayer;
	int pShotsFired;
	Vector CurrentViewAngles;
	Vector vPunch;
	Vector NewViewAngles;
	Vector OldAimPunch;
	Vector random;
	OldAimPunch.x = OldAimPunch.y = OldAimPunch.z = 0;

	pLocalPlayer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	vPunch = m->ReadMem<Vector>(pLocalPlayer + offsets::m_viewPunchAngle);
	pShotsFired = m->ReadMem<int>(pLocalPlayer + offsets::m_iShotsFired);

	if (pShotsFired > 1)
	{
		ClientState = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::dwClientState);
		CurrentViewAngles = m->ReadMem<Vector>(ClientState + offsets::dwClientState_ViewAngles);
		
		NewViewAngles.x = ((CurrentViewAngles.x + OldAimPunch.x) - (vPunch.x * 2.f));
		NewViewAngles.y = ((CurrentViewAngles.y + OldAimPunch.y) - (vPunch.y * 2.f));
		NewViewAngles.z = 0;

		//OldAimPunch.x = vPunch.x * 2.f;
		//OldAimPunch.y = vPunch.y * 2.f;
		//OldAimPunch.z = 0;

		//m->WriteMem<Vector>(m->eDll.dwBase + offsets::dwClientState + offsets::dwClientState_ViewAngles, NewViewAngles);
		Sleep(1);
	}

}

void Aimbot()
{
	if (GetAsyncKeyState(VK_F6))
	{
		aToggle = !aToggle;
		if (aToggle) std::cout << "Aimbot ON" << std::endl;
		else std::cout << "Aimbot OFF" << std::endl;
		Sleep(200);
	}


}

void fovChanger()
{
	DWORD dwPla = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);

	if (GetAsyncKeyState(VK_NUMPAD1))
	{
		fovToggle = !fovToggle;
		if (fovToggle) std::cout << "FOV Changer is ON" << std::endl;
		else std::cout << "FOV Changer is OFF" << std::endl;
		Sleep(200);
	}
	if (!fovToggle)
	{
		return;
	}
	
	int isScoped = m->ReadMem<int>(dwPla + offsets::isScoped);

	if(isScoped == 0)m->WriteMem<int>(dwPla + offsets::m_iFOVStart-4, 110);
	else if (isScoped > 0) m->WriteMem<int>(dwPla + offsets::m_iFOVStart - 4, 90);

}

void noFlash()
{
	if (GetAsyncKeyState(VK_F5))
	{
		fToggle = !fToggle;
		if(fToggle) std::cout << "No Flash is ON" << std::endl;
		else std::cout << "No Flash is OFF" << std::endl;
		Sleep(200);
	}
	DWORD dwPla = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	if (!fToggle)
	{
		m->WriteMem<float>(dwPla + offsets::flashAlpha, 255.f);
		return;
	}
	if (m->ReadMem<float>(dwPla + offsets::flashAlpha) > 0.0f) m->WriteMem<float>(dwPla + offsets::flashAlpha, 0.0f);
}

void visGlow()
{

	if (GetAsyncKeyState(VK_F4))
	{
		vgToggle = !vgToggle;
		if (vgToggle) std::cout << "Visible Glow is ON" << std::endl;
		else std::cout << "Visible Glow is OFF" << std::endl;
		Sleep(200);
	}
	DWORD gPointer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::dwGlow);
	DWORD dwPla = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	int lTeam = m->ReadMem<int>(dwPla + offsets::teamNum);
	if (!vgToggle) return;
	for (int i = 1; i < 64; i++)
	{
		int pEntity = m->ReadMem<int>(m->cDll.dwBase + offsets::entityList + i * 0x10);
		int pEntIdx = m->ReadMem<int>(pEntity + offsets::dwGlowIdx);

		int pEntTeam = m->ReadMem<int>(pEntity + offsets::teamNum);
		bool dormant = m->ReadMem<int>(pEntity + offsets::bDormant);

		if (dormant) continue;
		if (lTeam == pEntTeam) continue;

		glow_t vGlowObj = m->ReadMem<glow_t>(gPointer + (pEntIdx * 0x38));
		vGlowObj.r = .176f;
		vGlowObj.m_flGlowAlpha = 1.f;
		vGlowObj.g = .015f;
		vGlowObj.b = .337f;
		vGlowObj.m_bRenderWhenOccluded = false;
		vGlowObj.m_bRenderWhenUnoccluded = true;
		vGlowObj.m_nGlowStyle = 1;


		m->WriteMem(gPointer + (pEntIdx * 0x38), vGlowObj);
		Sleep(1);
	}
}

void Glow()
{

	if (GetAsyncKeyState(VK_F3))
	{
		gToggle = !gToggle;
		if (gToggle) std::cout << "Glow is ON" << std::endl;
		else std::cout << "Glow is OFF" << std::endl;
		Sleep(200);
	}
	DWORD gPointer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::dwGlow);
	DWORD dwPla = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	int lTeam = m->ReadMem<int>(dwPla + offsets::teamNum);
	if (!gToggle) return;
	for (int i = 1; i < 64; i++)
	{
		int pEntity = m->ReadMem<int>(m->cDll.dwBase + offsets::entityList + i * 0x10);
		int pEntIdx = m->ReadMem<int>(pEntity + offsets::dwGlowIdx);

		int pEntTeam = m->ReadMem<int>(pEntity + offsets::teamNum);
		bool dormant = m->ReadMem<int>(pEntity + offsets::bDormant);

		if (dormant) continue;
		if (lTeam == pEntTeam) continue;

		glow_t glowObj = m->ReadMem<glow_t>(gPointer + (pEntIdx * 0x38));

		glowObj.r = .176f;
		glowObj.m_flGlowAlpha = 1.f;
		glowObj.g = .015f;
		glowObj.b = .337f;
		glowObj.m_bRenderWhenOccluded = true;
		glowObj.m_bRenderWhenUnoccluded = false;
		glowObj.m_bFullBloomRender = false;
		glowObj.m_nGlowStyle = 0;


		m->WriteMem(gPointer + (pEntIdx * 0x38), glowObj);
		Sleep(1);
	}
	
}

void Bhop()
{
	if (GetAsyncKeyState(VK_F7))
	{
		bToggle = !bToggle;
		if (bToggle) std::cout << "Bhop is ON" << std::endl;
		else std::cout << "Bhop is OFF" << std::endl;
		Sleep(200);
	}
	if (!bToggle) return;
	DWORD dwPla = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	int flags = m->ReadMem<DWORD>(dwPla + 0x100);
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (flags & 0x1 == 1))
	{
		m->WriteMem<int>(m->cDll.dwBase + offsets::dwForceJump, 6);
		Sleep(1);
	}
}


void Trigger()
{
	if (GetAsyncKeyState(VK_F2))
	{
		tToggle = !tToggle;
		if (tToggle) std::cout << "Triggerbot is ON" << std::endl;
		else std::cout << "Triggerbot is OFF" << std::endl;
		Sleep(200);
	}
	
	if (!tToggle) return;
	DWORD dwPla = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	int lInCross = m->ReadMem<int>(dwPla + offsets::crosshairID) - 1;
	int lTeam = m->ReadMem<int>(dwPla + offsets::teamNum);
	int vecVel = m->ReadMem<int>(dwPla + offsets::vecVel);
	DWORD tEntityBase = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::entityList + ((lInCross) * 0x10));
	int tEntityTeam = m->ReadMem<int>(tEntityBase + offsets::teamNum);
	bool tDormant = m->ReadMem<bool>(tEntityBase + offsets::bDormant);
	
	if ((lInCross > 0 && lInCross <= 64) && (tEntityBase != NULL) && (tEntityTeam != lTeam) && (!tDormant) && (vecVel == 0))
	{
		Sleep(25);
		m->WriteMem<int>(m->cDll.dwBase + offsets::dwForceAttack, 6);
	}

}

void updateOffsets()
{
	DWORD LocalPlayerArray = m->FindPatternArray(m->cDll.dwBase, m->cDll.dwSize, "xxx????xx????xxxxx?", 19, 0x8D, 0x34, 0x85, 0x0, 0x0, 0x0, 0x0, 0x89, 0x15, 0x0, 0x0, 0x0, 0x0, 0x8B, 0x41, 0x8, 0x8B, 0x48, 0x0);
	offsets::localPlayer = m->ReadMem<DWORD>(LocalPlayerArray + 3) + m->ReadMem<BYTE>(LocalPlayerArray + 18) - m->cDll.dwBase;
	DWORD EntityListArray = m->FindPatternArray(m->cDll.dwBase, m->cDll.dwSize, "x????xx?xxx", 11, 0x5, 0x0, 0x0, 0x0, 0x0, 0xC1, 0xE9, 0x0, 0x39, 0x48, 0x4);
	offsets::entityList = (m->ReadMem<DWORD>(EntityListArray + 1) + m->ReadMem<BYTE>(EntityListArray + 7)) - m->cDll.dwBase;
	DWORD ForceJumpArray = m->FindPatternArray(m->cDll.dwBase, m->cDll.dwSize, "xx????xxxxxxx", 13, 0x8B, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x8B, 0xD6, 0x8B, 0xC1, 0x83, 0xCA, 0x02);
	offsets::dwForceJump = m->ReadMem<DWORD>(ForceJumpArray + 2) - m->cDll.dwBase;
	DWORD ForceAttackArray = m->FindPatternArray(m->cDll.dwBase, m->cDll.dwSize, "xx????xx????xxxxxxx", 19, 0x89, 0x0D, 0x0, 0x0, 0x0, 0x0, 0x8B, 0x0D, 0x0, 0x0, 0x0, 0x0, 0x8B, 0xF2, 0x8B, 0xC1, 0x83, 0xCE, 0x04);
	offsets::dwForceAttack = m->ReadMem<DWORD>(ForceAttackArray + 2) - m->cDll.dwBase;
	DWORD ClientStateArray = m->FindPatternArray(m->eDll.dwBase, m->eDll.dwSize, "x????xxxxxxxxxx", 15, 0xA1, 0x0, 0x0, 0x0, 0x0, 0x33, 0xD2, 0x6A, 0x0, 0x6A, 0x0, 0x33, 0xC9, 0x89, 0xB0);
	offsets::dwClientState = m->ReadMem<DWORD>(ClientStateArray + 1) - m->eDll.dwBase;
	DWORD GlowObjectArray = m->FindPatternArray(m->cDll.dwBase, m->cDll.dwSize, "x????xxxx", 9, 0xA1, 0x0, 0x0, 0x0, 0x0, 0xA8, 0x01, 0x75, 0x4B);
	offsets::dwGlow = m->ReadMem<DWORD>(GlowObjectArray + 1) + 0x4 - m->cDll.dwBase;
	DWORD ClientState_ViewAnglesArray = m->FindPatternArray(m->eDll.dwBase, m->eDll.dwSize, "xxx????xxxxx", 13, 0xF3, 0x0F, 0x11, 0x80, 0x0, 0x0, 0x0, 0x0, 0xD9, 0x46, 0x04, 0xD9, 0x05);
	offsets::dwClientState_ViewAngles = m->ReadMem<DWORD>(ClientState_ViewAnglesArray + 4);
}

int main()
{
	m = new MemManager();
	std::cout << "Elixir Free made by nci" << std::endl;
	std::cout << "Base address: 0x" << std::hex << m->cDll.dwBase << std::endl;
	updateOffsets();
	std::cout << "Glow Pointer: 0x" << std::hex << offsets::dwGlow << std::endl;
	std::cout << "Local Player: 0x" << std::hex << offsets::localPlayer << std::endl;
	std::cout << "Entity List: 0x" << std::hex << offsets::entityList << std::endl;
	std::cout << "Client State: 0x" << std::hex << offsets::dwClientState << std::endl;
	std::cout << "ForceJump: 0x" << std::hex << offsets::dwForceJump << std::endl;
	std::cout << "ForceAttack: 0x" << std::hex << offsets::dwForceAttack << std::endl;
	std::cout << "CState Viewangles: 0x" << std::hex << offsets::dwClientState_ViewAngles << std::endl;

	std::cout << "\n";
	std::cout << "Trigger is VK_F2 \n";
	std::cout << "Glow is VK_F3 \n";
	std::cout << "Visible Only Glow is VK_F4 \n";
	std::cout << "No Flash is VK_F5 \n";
	std::cout << "Bhop is VK_F7 \n";
	//std::cout << "RCS is VK_F8 \n";
	std::cout << "Chams is VK_F9 \n";
	std::cout << "Radar is VK_F10 \n";
	//std::cout << "FOV Changer is VK_NUMPAD1 \n";
	std::cout << "\n";
	while (true)
	{
		Trigger();
		Bhop();
		Glow();
		visGlow();
		chams();
		radar();
		noFlash();
		//RCS();
		//fovChanger();
	}
	delete m;
	return 0;
}