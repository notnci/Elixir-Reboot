#include "MemManager.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include "offsets.h"
#include "Globals.h"
#include "CBaseEntity.h"
MemManager* m;

void radar()
{
	if (GetAsyncKeyState(VK_F10))
	{
		G::Toggles::radToggle = !G::Toggles::radToggle;
		if (G::Toggles::radToggle) std::cout << "Radar ON" << std::endl;
		else std::cout << "Radar OFF" << std::endl;
		Sleep(200);
	}
	if (!G::Toggles::radToggle) return;
	auto dwLocalPlayer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	auto m_iLocalTeam = pEntity->GetTeamNum(dwLocalPlayer);
	for (auto i = 1; i < 64; i++)
	{
		auto dwEntity = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::entityList + i * 0x10);
		auto m_iEntity_Team = pEntity->GetTeamNum(dwEntity);
		//bool dormant = m->ReadMem<int>(pEntity + offsets::bDormant);

		if (m_iEntity_Team == m_iLocalTeam) continue;
		//if (dormant) continue;

		m->WriteMem<int>(dwEntity + offsets::m_bSpotted, 1);
		Sleep(1);
	}
}

void chams()
{
	if (GetAsyncKeyState(VK_F9))
	{
		G::Toggles::cToggle = !G::Toggles::cToggle;
		if (G::Toggles::cToggle) std::cout << "Chams ON" << std::endl;
		else std::cout << "Chams OFF" << std::endl;
		Sleep(200);
	}
	Structs::Color obj;
	if (!G::Toggles::cToggle) return;
	auto dwLocalPlayer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	auto m_iLocalTeam = pEntity->GetTeamNum(dwLocalPlayer);
	for (int i = 1; i < 64; i++)
	{
		auto dwEntity = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::entityList + i * 0x10);
		auto m_iEntity_Team = pEntity->GetTeamNum(dwEntity);
		

		if (m_iEntity_Team == m_iLocalTeam) continue;

		obj.r = 45;
		obj.g = 4;
		obj.b = 86;
		obj.a = 255;

		m->WriteMem<Structs::Color>(dwEntity + 0x70, obj);
		Sleep(1);
	}
}

//void RCS()
//{
	//if (GetAsyncKeyState(VK_F8))
	//{
		//G::Toggles::rToggle = !G::Toggles::rToggle;
		//if (G::Toggles::rToggle) std::cout << "RCS ON" << std::endl;
		//else std::cout << "RCS OFF" << std::endl;
		//Sleep(200);
	//}
	//if (!G::Toggles::rToggle) return;

	//DWORD ClientState;
	//DWORD pLocalPlayer;
	//int pShotsFired;
	//Structs::Vector CurrentViewAngles;
	//Structs::Vector vPunch;
	//Structs::Vector NewViewAngles;
	//Structs::Vector OldAimPunch;
	//Structs::Vector random;
	//OldAimPunch.x = OldAimPunch.y = OldAimPunch.z = 0;

	//pLocalPlayer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	//vPunch = m->ReadMem<Structs::Vector>(pLocalPlayer + offsets::m_viewPunchAngle);
	//pShotsFired = m->ReadMem<int>(pLocalPlayer + offsets::m_iShotsFired);

	//if (pShotsFired > 1)
	//{
		//ClientState = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::dwClientState);
		//CurrentViewAngles = m->ReadMem<Structs::Vector>(ClientState + offsets::dwClientState_ViewAngles);
		
		//NewViewAngles.x = ((CurrentViewAngles.x + OldAimPunch.x) - (vPunch.x * 2.f));
		//NewViewAngles.y = ((CurrentViewAngles.y + OldAimPunch.y) - (vPunch.y * 2.f));
		//NewViewAngles.z = 0;

		//OldAimPunch.x = vPunch.x * 2.f;
		//OldAimPunch.y = vPunch.y * 2.f;
		//OldAimPunch.z = 0;

		//m->WriteMem<Vector>(m->eDll.dwBase + offsets::dwClientState + offsets::dwClientState_ViewAngles, NewViewAngles);
		//Sleep(1);
	//}

//}


void noFlash()
{
	if (GetAsyncKeyState(VK_F5))
	{
		G::Toggles::fToggle = !G::Toggles::fToggle;
		if(G::Toggles::fToggle) std::cout << "No Flash is ON" << std::endl;
		else std::cout << "No Flash is OFF" << std::endl;
		Sleep(200);
	}
	auto dwLocalPlayer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	if (!G::Toggles::fToggle)
	{
		m->WriteMem<float>(dwLocalPlayer + offsets::flashAlpha, 255.f);
		return;
	}
	if (m->ReadMem<float>(dwLocalPlayer + offsets::flashAlpha) > 0.0f) m->WriteMem<float>(dwLocalPlayer + offsets::flashAlpha, 0.0f);
}

void visGlow()
{

	if (GetAsyncKeyState(VK_F4))
	{
		G::Toggles::vgToggle = !G::Toggles::vgToggle;
		if (G::Toggles::vgToggle) std::cout << "Visible Glow is ON" << std::endl;
		else std::cout << "Visible Glow is OFF" << std::endl;
		Sleep(200);
	}
	auto dwGlowPointer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::dwGlow);
	auto dwLocalPlayer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	auto m_iLocalTeam = pEntity->GetTeamNum(dwLocalPlayer);
	if (!G::Toggles::vgToggle) return;
	for (int i = 1; i < 64; i++)
	{
		auto dwEntity = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::entityList + i * 0x10);
		auto iEntityIndex = pEntity->GetEntityIndex(dwEntity);

		auto m_iEntity_Team = pEntity->GetTeamNum(dwEntity);

		if (m_iLocalTeam == m_iEntity_Team) continue;

		Structs::glow_t vGlowObj = m->ReadMem<Structs::glow_t>(dwGlowPointer + (iEntityIndex * 0x38));
		vGlowObj.r = .176f;
		vGlowObj.m_flGlowAlpha = 1.f;
		vGlowObj.g = .015f;
		vGlowObj.b = .337f;
		vGlowObj.m_bRenderWhenOccluded = false;
		vGlowObj.m_bRenderWhenUnoccluded = true;
		vGlowObj.m_nGlowStyle = 1;


		m->WriteMem(dwGlowPointer + (iEntityIndex * 0x38), vGlowObj);
		
	}
	Sleep(1);
}

void Glow()
{

	if (GetAsyncKeyState(VK_F3))
	{
		G::Toggles::gToggle = !G::Toggles::gToggle;
		if (G::Toggles::gToggle) std::cout << "Glow is ON" << std::endl;
		else std::cout << "Glow is OFF" << std::endl;
		Sleep(200);
	}
	auto dwGlowPointer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::dwGlow);
	auto dwLocalPlayer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	auto m_iLocalTeam = pEntity->GetTeamNum(dwLocalPlayer);
	if (!G::Toggles::gToggle) return;
	for (int i = 1; i < 64; i++)
	{
		auto dwEntity = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::entityList + i * 0x10);
		auto iEntityIndex = pEntity->GetEntityIndex(dwEntity);

		auto m_iEntity_Team = pEntity->GetTeamNum(dwEntity);
		if (m_iLocalTeam == m_iEntity_Team) continue;

		Structs::glow_t glowObj = m->ReadMem<Structs::glow_t>(dwGlowPointer + (iEntityIndex * 0x38));

		glowObj.r = .176f;
		glowObj.m_flGlowAlpha = 1.f;
		glowObj.g = .015f;
		glowObj.b = .337f;
		glowObj.m_bRenderWhenOccluded = true;
		glowObj.m_bRenderWhenUnoccluded = false;
		glowObj.m_bFullBloomRender = false;
		glowObj.m_nGlowStyle = 0;


		m->WriteMem(dwGlowPointer + (iEntityIndex * 0x38), glowObj);
		
	}
	Sleep(1);
	
}

void Bhop()
{
	if (GetAsyncKeyState(VK_F7))
	{
		G::Toggles::bToggle = !G::Toggles::bToggle;
		if (G::Toggles::bToggle) std::cout << "Bhop is ON" << std::endl;
		else std::cout << "Bhop is OFF" << std::endl;
		Sleep(200);
	}
	if (!G::Toggles::bToggle) return;
	auto dwLocalPlayer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && !pEntity->GetInAir(dwLocalPlayer))
	{
		m->WriteMem<int>(m->cDll.dwBase + offsets::dwForceJump, 6);
	}
}


void Trigger()
{
	if (GetAsyncKeyState(VK_F2))
	{
		G::Toggles::tToggle = !G::Toggles::tToggle;
		if (G::Toggles::tToggle) std::cout << "Triggerbot is ON" << std::endl;
		else std::cout << "Triggerbot is OFF" << std::endl;
		Sleep(200);
	}
	
	if (!G::Toggles::tToggle) return;
	auto dwLocalPlayer = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::localPlayer);
	auto m_iInCross = m->ReadMem<int>(dwLocalPlayer + offsets::crosshairID) - 1;
	auto m_iLocalTeam = m->ReadMem<int>(dwLocalPlayer + offsets::teamNum);
	auto dwEntity = m->ReadMem<DWORD>(m->cDll.dwBase + offsets::entityList + ((m_iInCross) * 0x10));
	auto m_iEntity_Team = pEntity->GetTeamNum(dwEntity);
	
	if ((m_iInCross > 0 && m_iInCross <= 64) && (dwEntity != NULL) && (m_iEntity_Team != m_iLocalTeam))
	{
		Sleep(25);
		m->WriteMem<int>(m->cDll.dwBase + offsets::dwForceAttack, 6);
	}

}

void updateOffsets()
{
	std::ptrdiff_t LocalPlayerArray = m->FindPatternArray(m->cDll.dwBase, m->cDll.dwSize, "xxx????xx????xxxxx?", 19, 0x8D, 0x34, 0x85, 0x0, 0x0, 0x0, 0x0, 0x89, 0x15, 0x0, 0x0, 0x0, 0x0, 0x8B, 0x41, 0x8, 0x8B, 0x48, 0x0);
	offsets::localPlayer = m->ReadMem<DWORD>(LocalPlayerArray + 3) + m->ReadMem<BYTE>(LocalPlayerArray + 18) - m->cDll.dwBase;
	std::ptrdiff_t EntityListArray = m->FindPatternArray(m->cDll.dwBase, m->cDll.dwSize, "x????xx?xxx", 11, 0x5, 0x0, 0x0, 0x0, 0x0, 0xC1, 0xE9, 0x0, 0x39, 0x48, 0x4);
	offsets::entityList = (m->ReadMem<DWORD>(EntityListArray + 1) + m->ReadMem<BYTE>(EntityListArray + 7)) - m->cDll.dwBase;
	std::ptrdiff_t ForceJumpArray = m->FindPatternArray(m->cDll.dwBase, m->cDll.dwSize, "xx????xxxxxxx", 13, 0x8B, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x8B, 0xD6, 0x8B, 0xC1, 0x83, 0xCA, 0x02);
	offsets::dwForceJump = m->ReadMem<DWORD>(ForceJumpArray + 2) - m->cDll.dwBase;
	std::ptrdiff_t ForceAttackArray = m->FindPatternArray(m->cDll.dwBase, m->cDll.dwSize, "xx????xx????xxxxxxx", 19, 0x89, 0x0D, 0x0, 0x0, 0x0, 0x0, 0x8B, 0x0D, 0x0, 0x0, 0x0, 0x0, 0x8B, 0xF2, 0x8B, 0xC1, 0x83, 0xCE, 0x04);
	offsets::dwForceAttack = m->ReadMem<DWORD>(ForceAttackArray + 2) - m->cDll.dwBase;
	std::ptrdiff_t ClientStateArray = m->FindPatternArray(m->eDll.dwBase, m->eDll.dwSize, "x????xxxxxxxxxx", 15, 0xA1, 0x0, 0x0, 0x0, 0x0, 0x33, 0xD2, 0x6A, 0x0, 0x6A, 0x0, 0x33, 0xC9, 0x89, 0xB0);
	offsets::dwClientState = m->ReadMem<DWORD>(ClientStateArray + 1) - m->eDll.dwBase;
	std::ptrdiff_t GlowObjectArray = m->FindPatternArray(m->cDll.dwBase, m->cDll.dwSize, "x????xxxx", 9, 0xA1, 0x0, 0x0, 0x0, 0x0, 0xA8, 0x01, 0x75, 0x4B);
	offsets::dwGlow = m->ReadMem<DWORD>(GlowObjectArray + 1) + 0x4 - m->cDll.dwBase;
	std::ptrdiff_t ClientState_ViewAnglesArray = m->FindPatternArray(m->eDll.dwBase, m->eDll.dwSize, "xxx????xxxxx", 13, 0xF3, 0x0F, 0x11, 0x80, 0x0, 0x0, 0x0, 0x0, 0xD9, 0x46, 0x04, 0xD9, 0x05);
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