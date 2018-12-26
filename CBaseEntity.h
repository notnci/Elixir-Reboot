#pragma once
#include <Windows.h>
#include "MemManager.h"
#include "offsets.h"

class CBaseEntity
{
public:
	int GetHealth(DWORD entity);
	int GetTeamNum(DWORD entity);
	DWORD GetEntity(DWORD entity);
	int GetEntityIndex(DWORD entity);
	int GetFlags(DWORD entity);
	int GetMoveType(DWORD entity);
	bool GetInAir(DWORD entity);

	CBaseEntity();
	~CBaseEntity();
};

extern CBaseEntity* pEntity;